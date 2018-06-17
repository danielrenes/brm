#ifndef BRM_STRATEGY_H
#define BRM_STRATEGY_H

#include "typedefs.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <string>

namespace Brm
{
  class Strategy
  {
    protected:
      std::string _type;
      std::string _src;
      std::string _dst;
      int _full_backup_period;
      std::chrono::system_clock::time_point _last_full_backup;
    public:
      Strategy(std::string, std::string, std::string, int);
      const std::string& src() const;
      const std::string& dst() const;
      bool compare(const Strategy&) const;
      const std::string to_string() const;
      void backup();
      virtual bool pre_check() = 0;
      virtual void post_process();
  };

  class FullBackupStrategy : public Strategy
  {
    private:
      static const std::string TYPE;
    public:
      FullBackupStrategy(std::string, std::string, int);
      bool pre_check() override final;
  };

  class IncrementalStrategy : public Strategy
  {
    private:
      static const std::string TYPE;
      std::string _last_hash;
    public:
      IncrementalStrategy(std::string, std::string, int);
      bool pre_check() override final;
  };

  class DifferentialStrategy : public Strategy
  {
    private:
      static const std::string& TYPE;
      std::string _last_full_backup_hash;
    public:
      DifferentialStrategy(std::string, std::string, int);
      bool pre_check() override final;
      void post_process() override final;
  };

  bool operator==(const Strategy&, const Strategy&);
  bool operator==(const FullBackupStrategy&, const FullBackupStrategy&);
  bool operator==(const IncrementalStrategy&, const IncrementalStrategy&);
  bool operator==(const DifferentialStrategy&, const DifferentialStrategy&);
  
  bool operator!=(const Strategy&, const Strategy&);
  bool operator!=(const FullBackupStrategy&, const FullBackupStrategy&);
  bool operator!=(const IncrementalStrategy&, const IncrementalStrategy&);
  bool operator!=(const DifferentialStrategy&, const DifferentialStrategy&);

  std::ostream& operator<<(std::ostream&, const Strategy&);
  std::ostream& operator<<(std::ostream&, const FullBackupStrategy&);
  std::ostream& operator<<(std::ostream&, const IncrementalStrategy&);
  std::ostream& operator<<(std::ostream&, const DifferentialStrategy&);

  class StrategyFactory
  {
    public:
      static std::unique_ptr<Strategy> create_strategy(STRATEGY, std::string, std::string, int);
  };
}

#endif
