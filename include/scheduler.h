#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <chrono>
#include <map>
#include <memory>
#include <list>
#include <thread>

namespace Brm
{
  class Rule;

  class Runner;

  template<typename T>
  class ThreadSafeQueue;

  class SchedulableCollector
  {
    public:
      bool is_collectable(std::chrono::system_clock::time_point,
                          std::chrono::system_clock::time_point,
                          int);
      std::list<std::shared_ptr<Rule>> collect(const std::map<Rule, int>&,
                                               std::map<Rule, std::chrono::system_clock::time_point>&);
  };

  class Scheduler
  {
    private:
      ThreadSafeQueue<Rule>& _queue;
      std::unique_ptr<SchedulableCollector> _collector;
      std::map<Rule, int> _period_time_table;
      std::map<Rule, std::chrono::system_clock::time_point> _last_scheduled_table;
      std::thread _thread;
      bool _terminate;
    public:
      Scheduler(ThreadSafeQueue<Rule>&);
      Scheduler(const Scheduler&) = delete;
      Scheduler& operator=(const Scheduler&) = delete;
      ~Scheduler();
      void schedule();
  };
}

#endif
