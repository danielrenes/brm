#include <chrono>
#include <iostream>
#include <stdexcept>

#include "config.h"
#include "logger.h"
#include "queue.h"
#include "runner.h"
#include "scheduler.h"

int main()
{
  std::cout << "Logging level: ";

  #if LOG_LEVEL == 1
    std::cout << "INFO";
  #elif LOG_LEVEL == 2
    std::cout << "ERROR";
  #elif LOG_LEVEL == 3
    std::cout << "DEBUG";
  #else
    std::cout << "undefined";
  #endif

  std::cout << std::endl;

  Brm::ThreadSafeQueue<Brm::Rule> queue;

  try
  {
    Brm::Scheduler scheduler(queue);
    Brm::Runner runner(queue);

    while (true)
    {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
  catch (std::exception& e)
  {
    ERROR("Exiting");

    return 1;
  }

  return 0;
}