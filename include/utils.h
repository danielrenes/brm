#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <sstream>
#include <string>

namespace Brm
{
  std::string read_file(const std::string&);

  void copy_file(const std::string&, const std::string&);

  std::string md5_checksum(std::string);

  int elapsed_minutes(std::chrono::system_clock::time_point from,
                      std::chrono::system_clock::time_point to);
  
  class StringBuilder
  {
    private:
      std::stringstream _ss;
    public:
      template<typename T>
      StringBuilder& operator<<(const T& t)
      {
        _ss << t;
        return *this;
      }
      operator std::string() const
      {
        return _ss.str();
      }
  };
}

#endif
