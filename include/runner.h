#ifndef RUNNER_H
#define RUNNER_H

#include <list>
#include <memory>
#include <thread>

namespace Brm
{
  class Rule;

  class Strategy;

  class Scheduler;

  template<typename T>
  class ThreadSafeQueue;

  class Runner
  {
    private:
      std::list<std::unique_ptr<Strategy>> _strategies;
      ThreadSafeQueue<Rule>& _queue;
      std::thread _thread;
      bool _terminate;
    public:
      Runner(ThreadSafeQueue<Rule>&);
      Runner(const Runner&) = delete;
      Runner& operator=(const Runner&) = delete;
      ~Runner();
      void update_strategies(std::shared_ptr<Rule>);
      void do_backup();
  };
}

#endif
