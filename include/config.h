#ifndef BRM_CONFIG_H
#define BRM_CONFIG_H

#include "typedefs.h"

#include <list>
#include <memory>
#include <string>

namespace Brm
{
  class Location
  {
    private:
      std::string _original;
      std::string _backup;
    public:
      Location(std::string, std::string);
      const std::string& original() const;
      const std::string& backup() const;
  };

  class Rule
  {
    private:
      static int _next_id;
      const int _id;
      Period _period;
      Period _full_backup_period;
      STRATEGY _strategy;
      std::list<std::shared_ptr<Location>> _locations;
    public:
      Rule(Period, Period, STRATEGY);
      Rule(const Rule&);
      ~Rule();
      void add_location(std::shared_ptr<Location>);
      const int& id() const;
      const Period& period() const;
      const Period& full_backup_period() const;
      const STRATEGY& strategy() const;
      const std::list<std::shared_ptr<Location>>& locations() const;
  };

  bool operator<(const Rule&, const Rule&);

  class Configuration
  {
    private:
      std::list<std::shared_ptr<Rule>> _rules;
    public:
      Configuration();
      Configuration(const Configuration&) = delete;
      Configuration& operator= (const Configuration&) = delete;
      ~Configuration();
      void add_rule(std::shared_ptr<Rule>);
      const std::list<std::shared_ptr<Rule>>& rules() const;
  };

  class ConfigurationManager
  {
    private:
      static const char* CONFIG_FILE_PATH;
      ConfigurationManager();
      std::unique_ptr<Configuration> _configuration;
      std::unique_ptr<Configuration> parse_configuration();
    public:
      ConfigurationManager(const ConfigurationManager&) = delete;
      ConfigurationManager& operator=(const ConfigurationManager&) = delete;
      static ConfigurationManager& instance();
      const Configuration* configuration() const;
  };
}

#endif
