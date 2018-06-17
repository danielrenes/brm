#include <string>

#include <gtest/gtest.h>

#include <iostream>

#include "strategy.h"
#include "typedefs.h"
#include "utils.h"

TEST(StrategyTest, FullBackupToString)
{
  Brm::FullBackupStrategy bs{std::string{"src"}, std::string{"dst"}, 1};
  ASSERT_STREQ(((std::string) (Brm::StringBuilder() << bs)).c_str(), "Full Backup (src > dst)");
}

TEST(StrategyTest, IncrementalToString)
{
  Brm::IncrementalStrategy bs{std::string{"src"}, std::string{"dst"}, 1};
  ASSERT_STREQ(((std::string) (Brm::StringBuilder() << bs)).c_str(), "Incremental Backup (src > dst)");
}

TEST(StrategyTest, DifferentialToString)
{
  Brm::DifferentialStrategy bs{std::string{"src"}, std::string{"dst"}, 1};
  ASSERT_STREQ(((std::string) (Brm::StringBuilder() << bs)).c_str(), "Differential Backup (src > dst)");
}

TEST(StrategyTest, StrategyFactory)
{
  auto strategy = Brm::StrategyFactory::create_strategy(Brm::STRATEGY::FULL_BACKUP,
                                                        std::string{"src"},
                                                        std::string{"dst"},
                                                        1);
  ASSERT_STREQ(((std::string) (Brm::StringBuilder() << *(strategy.get()))).c_str(), "Full Backup (src > dst)");
}

TEST(StrategyTest, Comparison)
{
  auto strategy1 = Brm::StrategyFactory::create_strategy(Brm::STRATEGY::FULL_BACKUP,
                                                         std::string{"src"},
                                                         std::string{"dst"},
                                                         1);
  auto strategy2 = Brm::StrategyFactory::create_strategy(Brm::STRATEGY::FULL_BACKUP,
                                                         std::string{"src"},
                                                         std::string{"dst"},
                                                         1);

  ASSERT_TRUE(*(strategy1.get()) == *(strategy2.get()));
}