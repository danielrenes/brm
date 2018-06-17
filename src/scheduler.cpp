#include "scheduler.h"

#include <chrono>
#include <stdexcept>
#include <thread>

#include "config.h"
#include "logger.h"
#include "queue.h"
#include "runner.h"
#include "typedefs.h"
#include "utils.h"

namespace Brm
{
  bool SchedulableCollector::is_collectable(std::chrono::system_clock::time_point now,
                                            std::chrono::system_clock::time_point last_backup,
                                            int period_time)
  {
    int elapsed = elapsed_minutes(last_backup, now);
    bool is_collected = (elapsed >= period_time);

    DEBUG(StringBuilder() << "Now: " << std::chrono::system_clock::to_time_t(now)
                          << " Last backup: " << std::chrono::system_clock::to_time_t(last_backup)
                          << " Period time: " << period_time << " mins"
                          << " Scheduled: " << (is_collected ? "yes" : "no"));

    return is_collected;
  }

  std::list<std::shared_ptr<Rule>> SchedulableCollector::collect(const std::map<Rule, int>& periods,
                                                                 std::map<Rule, std::chrono::system_clock::time_point>& times)
  {
    std::list<std::shared_ptr<Rule>> collection;

    auto now = std::chrono::system_clock::now();

    auto period_iterator = periods.cbegin();
    auto time_iterator = times.begin();

    while (period_iterator != periods.cend() &&
           time_iterator != times.end())
    {
      if (is_collectable(now, time_iterator->second, period_iterator->second))
      {
        time_iterator->second = now;
        collection.push_back(std::make_shared<Rule>(time_iterator->first));
      }

      period_iterator++;
      time_iterator++;
    }

    return collection;
  }

  Scheduler::Scheduler(ThreadSafeQueue<Rule>& queue)
    : _queue{queue},
      _thread{},
      _terminate{false}
  {
    auto configuration = ConfigurationManager::instance().configuration();

    _thread = std::thread{&Scheduler::schedule, this};

    if (configuration != nullptr)
    {
      auto now = std::chrono::system_clock::now();

      for (auto const rule : configuration->rules())
      {
        const int period_time = rule->period().to_minutes();

        _period_time_table[*rule] = period_time;
        _last_scheduled_table[*rule] = now;

        DEBUG(StringBuilder() << "Added rule (Period time: " << period_time << " mins"
                              << " Full backup period time: " << rule->full_backup_period().to_minutes() << " mins)");
      }
    }
  }

  Scheduler::~Scheduler()
  {
    _terminate = true;

    if (_thread.joinable())
    {
      _thread.join();
      
      INFO("Stopped scheduler thread");
    }
  }

  void Scheduler::schedule()
  {
    INFO("Started scheduler thread");
    while (!_terminate)
    {
      auto scheduled = _collector->collect(_period_time_table, _last_scheduled_table);
      if (scheduled.size() > 0)
      {
        for (auto rule : scheduled)
        {
          _queue.addOne(rule);
        }
      }
      else
      {
        DEBUG("Nothing to backup");
      }
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
}
