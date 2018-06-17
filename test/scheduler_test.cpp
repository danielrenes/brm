#include <chrono>
#include <memory>

#include <gtest/gtest.h>

#include "config.h"
#include "runner.h"
#include "scheduler.h"

TEST(CollectorTest, ShouldCollectTrue)
{
  auto now = std::chrono::system_clock::from_time_t(10000);
  auto last_backup = std::chrono::system_clock::from_time_t(6400);
  int period_time = 60;
  Brm::SchedulableCollector collector;
  ASSERT_TRUE(collector.is_collectable(now, last_backup, period_time));
}

TEST(CollectorTest, ShouldCollectFalse)
{
  auto now = std::chrono::system_clock::from_time_t(10000);
  auto last_backup = std::chrono::system_clock::from_time_t(6400);
  int period_time = 61;
  Brm::SchedulableCollector collector;
  ASSERT_FALSE(collector.is_collectable(now, last_backup, period_time));
}
