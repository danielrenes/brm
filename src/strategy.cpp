#include "strategy.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "logger.h"
#include "typedefs.h"
#include "utils.h"

namespace Brm
{
  Strategy::Strategy(std::string type,
                     std::string src,
                     std::string dst,
                     int full_backup_period)
    : _type{type},
      _src{src},
      _dst{dst},
      _full_backup_period{full_backup_period},
      _last_full_backup{std::chrono::system_clock::now()}
  {
  }

  const std::string& Strategy::src() const
  {
    return _src;
  }

  const std::string& Strategy::dst() const
  {
    return _dst;
  }

  bool Strategy::compare(const Strategy& other) const
  {
    return (_type == other._type &&
            _src == other._src &&
            _dst == other._dst &&
            _full_backup_period == other._full_backup_period);
  }

  const std::string Strategy::to_string() const
  {
    return (StringBuilder() << _type << " (" << _src << " > " << _dst << ")");
  }

  void Strategy::post_process()
  {
  }

  void Strategy::backup()
  {
    auto now = std::chrono::system_clock::now();
    int elapsed = elapsed_minutes(_last_full_backup, now);

    if (elapsed >= _full_backup_period)
    {
      copy_file(_src, _dst);

      _last_full_backup = std::chrono::system_clock::now();

      post_process();

      INFO(StringBuilder() << "Backed up " << _src << " to " << _dst);
    }
    else if (pre_check())
    {
      copy_file(_src, _dst);

      INFO(StringBuilder() << "Backed up " << _src << " to " << _dst);
    }
  }

  const std::string FullBackupStrategy::TYPE = std::string{"Full Backup"};

  FullBackupStrategy::FullBackupStrategy(std::string src,
                                         std::string dst,
                                         int full_backup_period)
    : Strategy(TYPE, src, dst, full_backup_period)
  {
  }

  bool FullBackupStrategy::pre_check()
  {
    return false;
  }

  const std::string IncrementalStrategy::TYPE = std::string{"Incremental Backup"};

  IncrementalStrategy::IncrementalStrategy(std::string src,
                                           std::string dst,
                                           int full_backup_period)
    : Strategy(TYPE, src, dst, full_backup_period)
  {
    _last_hash = "";
  }

  bool IncrementalStrategy::pre_check()
  {
    std::string hash = md5_checksum(read_file(_src));

    if (_last_hash == "" || _last_hash != hash)
    {
      DEBUG("Incremental backup");

      _last_hash = hash;

      return true;
    }

    return false;
  }

  const std::string& DifferentialStrategy::TYPE = std::string{"Differential Backup"};

  DifferentialStrategy::DifferentialStrategy(std::string src,
                                             std::string dst,
                                             int full_backup_period)
    : Strategy(TYPE, src, dst, full_backup_period)
  {
    _last_full_backup_hash = "";
  }

  bool DifferentialStrategy::pre_check()
  {
    std::string hash = md5_checksum(read_file(_src));

    if (_last_full_backup_hash == "" || _last_full_backup_hash != hash)
    {
      DEBUG("Differential backup");

      return true;
    }

    return false;
  }

  void DifferentialStrategy::post_process()
  {
    _last_full_backup_hash = md5_checksum(read_file(_dst));
  }

  bool operator==(const Strategy& lhs, const Strategy& rhs)
  {
    return lhs.compare(rhs);
  }

  bool operator==(const FullBackupStrategy& lhs, const FullBackupStrategy& rhs)
  {
    return lhs.compare(rhs);
  }

  bool operator==(const IncrementalStrategy& lhs, const IncrementalStrategy& rhs)
  {
    return lhs.compare(rhs);
  }

  bool operator==(const DifferentialStrategy& lhs, const DifferentialStrategy& rhs)
  {
    return lhs.compare(rhs);
  }

  bool operator!=(const Strategy& lhs, const Strategy& rhs)
  {
    return !lhs.compare(rhs);
  }

  bool operator!=(const FullBackupStrategy& lhs, const FullBackupStrategy& rhs)
  {
    return !lhs.compare(rhs);
  }

  bool operator!=(const IncrementalStrategy& lhs, const IncrementalStrategy& rhs)
  {
    return !lhs.compare(rhs);
  }

  bool operator!=(const DifferentialStrategy& lhs, const DifferentialStrategy& rhs)
  {
    return !lhs.compare(rhs);
  }

  std::ostream& operator<<(std::ostream& os, const Strategy& s)
  {
    os << s.to_string();
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const FullBackupStrategy& s)
  {
    os << s.to_string();
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const IncrementalStrategy& s)
  {
    os << s.to_string();
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const DifferentialStrategy& s)
  {
    os << s.to_string();
    return os;
  }

  std::unique_ptr<Strategy> StrategyFactory::create_strategy(STRATEGY strategy,
                                                             std::string src,
                                                             std::string dst,
                                                             int full_backup_period)
  {
    std::unique_ptr<Strategy> s;

    if (strategy == STRATEGY::FULL_BACKUP)
    {
      s.reset(new FullBackupStrategy(src, dst, full_backup_period));
    }
    else if (strategy == STRATEGY::INCREMENTAL)
    {
      s.reset(new IncrementalStrategy(src, dst, full_backup_period));
    }
    else if (strategy == STRATEGY::DIFFERENTIAL)
    {
      s.reset(new DifferentialStrategy(src, dst, full_backup_period));
    }

    return s;
  }
}
