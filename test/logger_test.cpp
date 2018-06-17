#include <regex>
#include <streambuf>
#include <string>
#include <sstream>

#include <iostream>

#include <gtest/gtest.h>

#include "logger.h"

TEST(LoggerTest, TimeFormat)
{
  const std::regex datetime_pattern{"^\\d{4}-\\d{2}-\\d{2}\\s\\d{2}:\\d{2}:\\d{2}$"};
  std::stringstream buffer;
  std::streambuf* sb = std::cout.rdbuf(buffer.rdbuf());
  INFO("almafa");
  std::string msg = buffer.str();
  std::cout.rdbuf(sb);
  auto index = msg.find(' ');
  const std::string datetime = msg.substr(0, msg.find(' ', index + 1));
  std::smatch match;
  ASSERT_TRUE(std::regex_match(datetime, match, datetime_pattern));
}

TEST(LoggerTest, File)
{
  const std::string expected{"logger_test"};
  const std::regex file_pattern{"\\s(\\w+|_)::"};
  std::stringstream buffer;
  std::streambuf* sb = std::cout.rdbuf(buffer.rdbuf());
  INFO("almafa");
  const std::string msg = buffer.str();
  std::cout.rdbuf(sb);
  std::smatch match;
  ASSERT_TRUE(std::regex_search(msg.begin(), msg.end(), match, file_pattern));
  ASSERT_TRUE(match[1] == expected);
}

TEST(LoggerTest, Function)
{
  const std::string expected{"TestBody"};
  const std::regex function_pattern{"::(\\w+)\\("};
  std::stringstream buffer;
  std::streambuf* sb = std::cout.rdbuf(buffer.rdbuf());
  INFO("almafa");
  const std::string msg = buffer.str();
  std::cout.rdbuf(sb);
  std::smatch match;
  ASSERT_TRUE(std::regex_search(msg.begin(), msg.end(), match, function_pattern));
  ASSERT_TRUE(match[1] == expected);
}

TEST(LoggerTest, Line)
{
  const std::string expected{"60"};
  const std::regex line_pattern{"\\((\\d+)\\)"};
  std::stringstream buffer;
  std::streambuf* sb = std::cout.rdbuf(buffer.rdbuf());
  INFO("almafa");
  const std::string msg = buffer.str();
  std::cout.rdbuf(sb);
  std::smatch match;
  ASSERT_TRUE(std::regex_search(msg.begin(), msg.end(), match, line_pattern));
  ASSERT_TRUE(match[1] == expected);
}

TEST(LoggerTest, Level)
{
  const std::string expected{"INFO"};
  const std::regex level_pattern{"\\[(\\w+)\\]"};
  std::stringstream buffer;
  std::streambuf* sb = std::cout.rdbuf(buffer.rdbuf());
  INFO("almafa");
  const std::string msg = buffer.str();
  std::cout.rdbuf(sb);
  std::smatch match;
  ASSERT_TRUE(std::regex_search(msg.begin(), msg.end(), match, level_pattern));
  ASSERT_TRUE(match[1] == expected);
}

TEST(LoggerTest, Message)
{
  const std::string expected{"almafa\n"};
  std::stringstream buffer;
  std::streambuf* sb = std::cout.rdbuf(buffer.rdbuf());
  INFO("almafa");
  const std::string msg = buffer.str();
  std::cout.rdbuf(sb);
  const std::string message = msg.substr(msg.rfind(' ') + 1);
  ASSERT_TRUE(message == expected);
}

TEST(LoggerTest, InfoLevel1)
{
  #define LOG_LEVEL 1
  std::stringstream buffer;
  std::streambuf* sb = std::cout.rdbuf(buffer.rdbuf());
  INFO("almafa");
  const std::string msg = buffer.str();
  std::cout.rdbuf(sb);
  ASSERT_TRUE(msg.size() > 0);
}

TEST(LoggerTest, InfoLevel2)
{
  #define LOG_LEVEL 2
  std::stringstream buffer;
  std::streambuf* sb = std::cout.rdbuf(buffer.rdbuf());
  INFO("almafa");
  const std::string msg = buffer.str();
  std::cout.rdbuf(sb);
  ASSERT_TRUE(msg.size() > 0);
}

TEST(LoggerTest, InfoLevel3)
{
  #define LOG_LEVEL 1
  std::stringstream buffer;
  std::streambuf* sb = std::cout.rdbuf(buffer.rdbuf());
  INFO("almafa");
  const std::string msg = buffer.str();
  std::cout.rdbuf(sb);
  ASSERT_TRUE(msg.size() > 0);
}

TEST(LoggerTest, ErrorLevel1)
{
  #define LOG_LEVEL 1
  std::stringstream buffer;
  std::streambuf* sb = std::cout.rdbuf(buffer.rdbuf());
  ERROR("almafa");
  const std::string msg = buffer.str();
  std::cout.rdbuf(sb);
  ASSERT_TRUE(msg.size() == 0);
}

TEST(LoggerTest, ErrorLevel2)
{
  #define LOG_LEVEL 2
  std::stringstream buffer;
  std::streambuf* sb = std::cout.rdbuf(buffer.rdbuf());
  ERROR("almafa");
  const std::string msg = buffer.str();
  std::cout.rdbuf(sb);
  ASSERT_TRUE(msg.size() > 0);
}

TEST(LoggerTest, ErrorLevel3)
{
  #define LOG_LEVEL 3
  std::stringstream buffer;
  std::streambuf* sb = std::cout.rdbuf(buffer.rdbuf());
  ERROR("almafa");
  const std::string msg = buffer.str();
  std::cout.rdbuf(sb);
  ASSERT_TRUE(msg.size() > 0);
}

TEST(LoggerTest, DebugLevel1)
{
  #define LOG_LEVEL 1
  std::stringstream buffer;
  std::streambuf* sb = std::cout.rdbuf(buffer.rdbuf());
  DEBUG("almafa");
  const std::string msg = buffer.str();
  std::cout.rdbuf(sb);
  ASSERT_TRUE(msg.size() == 0);
}

TEST(LoggerTest, DebugLevel2)
{
  #define LOG_LEVEL 2
  std::stringstream buffer;
  std::streambuf* sb = std::cout.rdbuf(buffer.rdbuf());
  DEBUG("almafa");
  const std::string msg = buffer.str();
  std::cout.rdbuf(sb);
  ASSERT_TRUE(msg.size() == 0);
}

TEST(LoggerTest, DebugLevel3)
{
  #define LOG_LEVEL 3
  std::stringstream buffer;
  std::streambuf* sb = std::cout.rdbuf(buffer.rdbuf());
  DEBUG("almafa");
  const std::string msg = buffer.str();
  std::cout.rdbuf(sb);
  ASSERT_TRUE(msg.size() > 0);
}