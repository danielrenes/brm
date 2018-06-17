#include "runner.h"

#include <algorithm>
#include <chrono>
#include <list>
#include <memory>
#include <thread>

#include "config.h"
#include "logger.h"
#include "queue.h"
#include "strategy.h"
#include "scheduler.h"
#include "utils.h"

namespace Brm
{
  Runner::Runner(ThreadSafeQueue<Rule>& queue)
    : _queue{queue},
      _thread{&Runner::do_backup, this},
      _terminate{false}
  {
  }

  Runner::~Runner()
  {
    _terminate = true;

    if (_thread.joinable())
    {
      _thread.join();
      
      INFO("Stopped runner thread");
    }

    _strategies.clear();
  }

  void Runner::update_strategies(std::shared_ptr<Rule> rule)
  {
    for (auto const location : rule->locations())
    {
      std::unique_ptr<Strategy> strategy = StrategyFactory::create_strategy(rule->strategy(),
                                                                            location->original(),
                                                                            location->backup(),
                                                                            rule->full_backup_period().to_minutes());

      if (strategy == nullptr)
      {
        DEBUG("Strategy is undefined");
        break;
      }

      DEBUG(StringBuilder() << "Strategy created for rule " << rule->id());

      auto iterator = std::find_if(_strategies.begin(), _strategies.end(),
          [&strategy] (const std::unique_ptr<Strategy>& item)
          {
            return (item->src() == strategy->src()
                    && item->dst() == strategy->dst());
          }
      );

      if (iterator != _strategies.end())
      {
        DEBUG("Strategy exists");

        if (*((*iterator).get()) != *(strategy.get()))
        {
          INFO("Replacing strategy");
          DEBUG(StringBuilder() << "Old strategy: " << *((*iterator).get()));
          DEBUG(StringBuilder() << "New strategy: " << *(strategy.get()));

          _strategies.erase(iterator);
          _strategies.push_back(std::move(strategy));

          iterator->reset();
        }
      }
      else
      {
        DEBUG(StringBuilder() << "Strategy added: " << *(strategy.get()));

        _strategies.push_back(std::move(strategy));
      }
    }
  }

  void Runner::do_backup()
  {
    INFO("Started runner thread");
    while (!_terminate)
    {
      auto rule = _queue.getOne();

      update_strategies(rule);

      for (auto const& strategy : _strategies)
      {
        strategy->backup();
      }

      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
}
