#ifndef BRM_TYPEDEFS_H
#define BRM_TYPEDEFS_H

#include <string>
#include <stdexcept>

namespace Brm
{
  enum class TIME_UNIT
  {
    HOUR,
    DAY,
    WEEK,
    MONTH
  };

  enum class STRATEGY
  {
    FULL_BACKUP,
    INCREMENTAL,
    DIFFERENTIAL
  };

  enum class PLATFORM
  {
    WINDOWS,
    MAC,
    LINUX,
    UNKNOWN
  };

  class Period
  {
    private:
      int _value;
      TIME_UNIT _time_unit;
    public:
      Period() : _value{-1}, _time_unit{TIME_UNIT::HOUR}
      {
      }

      Period(int value, TIME_UNIT time_unit)
      {
        if (value < 0 ||
            (time_unit == TIME_UNIT::HOUR && value > 24) ||
            (time_unit == TIME_UNIT::DAY && value > 30) ||
            (time_unit == TIME_UNIT::WEEK && value > 4) ||
            (time_unit == TIME_UNIT::MONTH && value > 12))
        {
          throw std::invalid_argument("Value is out of range for time unit");
        }

        _value = value;
        _time_unit = time_unit;
      }

      Period(const Period& other) : Period{other._value, other._time_unit}
      {
      }

      const int& value() const
      {
        return _value;
      }

      const TIME_UNIT& time_unit() const
      {
        return _time_unit;
      }

      const int to_minutes() const
      {
        switch (_time_unit)
        {
          case TIME_UNIT::HOUR:
            return (_value * 60);
          case TIME_UNIT::DAY:
            return (_value * 60 * 24);
          case TIME_UNIT::WEEK:
            return (_value * 60 * 24 * 7);
          case TIME_UNIT::MONTH:
            return (_value * 60 * 24 * 30);
          default:
            return -1;
        }
      }
  };

  inline TIME_UNIT StringToTimeUnit(std::string str)
  {
    if (str == "hour")
    {
      return TIME_UNIT::HOUR;
    }
    else if (str == "day")
    {
      return TIME_UNIT::DAY;
    }
    else if (str == "week")
    {
      return TIME_UNIT::WEEK;
    }
    else if (str == "month")
    {
      return TIME_UNIT::MONTH;
    }
    throw std::invalid_argument("Undefined time unit");
  }

  inline const char* TimeUnitToString(TIME_UNIT time_unit)
  {
    if (time_unit == TIME_UNIT::HOUR)
    {
      return "hour";
    }
    else if (time_unit == TIME_UNIT::DAY)
    {
      return "day";
    }
    else if (time_unit == TIME_UNIT::WEEK)
    {
      return "week";
    }
    else if (time_unit == TIME_UNIT::MONTH)
    {
      return "month";
    }
    throw std::invalid_argument("Undefined time unit");
  }

  inline STRATEGY StringToStrategy(std::string str)
  {
    if (str == "full-backup")
    {
      return STRATEGY::FULL_BACKUP;
    }
    else if (str == "incremental")
    {
      return STRATEGY::INCREMENTAL;
    }
    else if (str == "differential")
    {
      return STRATEGY::DIFFERENTIAL;
    }
    throw std::invalid_argument("Undefined strategy");
  }

  inline const char* StrategyToString(STRATEGY strategy)
  {
    if (strategy == STRATEGY::FULL_BACKUP)
    {
      return "full-backup";
    }
    else if (strategy == STRATEGY::INCREMENTAL)
    {
      return "incremental";
    }
    else if (strategy == STRATEGY::DIFFERENTIAL)
    {
      return "differential";
    }
    throw std::invalid_argument("Undefined strategy");
  }
}

#endif
