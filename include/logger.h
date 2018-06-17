#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

#include "system.h"

#ifndef LOG_LEVEL
#define LOG_LEVEL 0
#endif

#define INFO(msg) (LOG_LEVEL >= 1 ? std::cout << log(__FILE__, __FUNCTION__, __LINE__, "INFO", msg) << std::endl : std::cout << "")

#define ERROR(msg) (LOG_LEVEL >= 2 ? std::cout << log(__FILE__, __FUNCTION__, __LINE__, "ERROR", msg) << std::endl : std::cout << "")

#define DEBUG(msg) (LOG_LEVEL >= 3 ? std::cout << log(__FILE__, __FUNCTION__, __LINE__, "DEBUG", msg) << std::endl : std::cout << "")

inline std::string now()
{
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
  return ss.str();
}

inline std::string trace(std::string file, std::string function, int line)
{
  const char delim = Brm::OS == Brm::PLATFORM::WINDOWS ? '\\' : '/';
  std::stringstream ss;
  const std::string filename = file.substr(file.rfind(delim) + 1);
  const std::string filename_without_extension = filename.substr(0, filename.rfind('.'));
  ss << filename_without_extension << "::" << function << "(" << line << ")";
  return ss.str();
}

inline std::string log(std::string file, std::string function, int line, std::string level, std::string msg)
{
  std::stringstream ss;
  ss << now() << " " << trace(file, function, line) << " [" << level << "] " << msg;
  return ss.str();
}

#endif