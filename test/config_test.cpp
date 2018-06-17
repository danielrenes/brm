#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "config.h"
#include "typedefs.h"

TEST(ConfigTest, Parser)
{
  std::vector<std::vector<const char*>> expected = {
    {"1", "hour", "1", "hour", "full-backup", "/src/orig_1", "/dst/back_1"},
    {"1", "day", "1", "week", "incremental", "/src/orig_2", "/dst/back_2"},
    {"1", "week", "1", "month", "differential", "/src/orig_3_1", "/dst/back_3_1", "/src/orig_3_2", "/dst/back_3_2"}
  };

  int outer_index = 0;

  auto const configuration = Brm::ConfigurationManager::instance().configuration();
  
  for (auto const rule : configuration->rules())
  {
    int inner_index = 0;

    ASSERT_STREQ(
      expected[outer_index][inner_index++],
      (std::to_string(rule->period().value())).c_str());
    ASSERT_STREQ(
      expected[outer_index][inner_index++],
      Brm::TimeUnitToString(rule->period().time_unit()));
    ASSERT_STREQ(
      expected[outer_index][inner_index++],
      (std::to_string(rule->full_backup_period().value())).c_str());
    ASSERT_STREQ(
      expected[outer_index][inner_index++],
      Brm::TimeUnitToString(rule->full_backup_period().time_unit()));
    ASSERT_STREQ(
      expected[outer_index][inner_index++],
      Brm::StrategyToString(rule->strategy()));

    for (auto const location : rule->locations())
    {
      ASSERT_STREQ(
        expected[outer_index][inner_index++],
        location->original().c_str());
      ASSERT_STREQ(
        expected[outer_index][inner_index++],
        location->backup().c_str());
    }

    outer_index++;
  }
}
