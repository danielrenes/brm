#include <string>

#include <gtest/gtest.h>

#include "utils.h"

TEST(UtilsTest, ReadFile)
{
  const std::string content = Brm::read_file("test/res/test_file.txt");
  ASSERT_STREQ(content.c_str(), "appletree\n");
}

TEST(UtilsTest, MD5Checksum)
{
  ASSERT_STREQ(Brm::md5_checksum("appletree").c_str(), Brm::md5_checksum("appletree").c_str());

  ASSERT_STRNE(Brm::md5_checksum("appletree").c_str(), Brm::md5_checksum("appletree2").c_str());
}

TEST(UtilsTest, StringBuilder)
{
  ASSERT_STREQ(((std::string)(Brm::StringBuilder() << "apple" << "tree")).c_str(), "appletree");
}
