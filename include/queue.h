#ifndef QUEUE_H
#define QUEUE_H

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

#include "logger.h"

namespace Brm
{
  template<typename T>
  class ThreadSafeQueue
  {
    private:
      std::queue<std::shared_ptr<T>> _queue;
      std::mutex _mutex;
      std::condition_variable _cond;
    public:
      ThreadSafeQueue()
        : _queue(), _mutex(), _cond()
      {
      }

      ~ThreadSafeQueue() {}
      
      void addOne(std::shared_ptr<T> t)
      {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(t);
        _cond.notify_one();

        DEBUG("Added one item to queue");
      }
      
      std::shared_ptr<T> getOne()
      {
        std::unique_lock<std::mutex> lock(_mutex);
        while(_queue.empty())
        {
          _cond.wait(lock);
        }
        std::shared_ptr<T> item = _queue.front();
        _queue.pop();

        DEBUG("Removed one item from queue");

        return item;
      }
  };
}

#endif