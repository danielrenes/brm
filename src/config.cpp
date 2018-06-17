#include "config.h"

#include <iostream>
#include <fstream>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>

#include <jsoncpp/json/json.h>

#include "logger.h"

namespace Brm
{
  Location::Location(std::string original, std::string backup)
    : _original{original}, _backup{backup}
  {
  }

  const std::string& Location::original() const
  {
    return _original;
  }
  const std::string& Location::backup() const
  {
    return _backup;
  }

  int Rule::_next_id = 0;

  Rule::Rule(Period period, Period full_backup_period, STRATEGY strategy)
    : _id{_next_id++},
      _period{period},
      _full_backup_period{full_backup_period},
      _strategy{strategy}
  {
  }

  Rule::Rule(const Rule& other)
    : _id{other._id},
      _period{other._period},
      _full_backup_period{other._full_backup_period},
      _strategy{other._strategy},
      _locations{std::list<std::shared_ptr<Location>>(other._locations)}
  {
  }

  Rule::~Rule()
  {
    _locations.clear();
  }

  void Rule::add_location(std::shared_ptr<Location> location)
  {
    _locations.push_back(location);
  }

  const int& Rule::id() const
  {
    return _id;
  }

  const Period& Rule::period() const
  {
    return _period;
  }

  const Period& Rule::full_backup_period() const
  {
    return _full_backup_period;
  }

  const STRATEGY& Rule::strategy() const
  {
    return _strategy;
  }

  const std::list<std::shared_ptr<Location>>& Rule::locations() const
  {
    return _locations;
  }

  bool operator<(const Rule& lhs, const Rule& rhs)
  {
    return (lhs.id() < rhs.id());
  }

  Configuration::Configuration()
  {
  }

  Configuration::~Configuration()
  {
    _rules.clear();
  }

  void Configuration::add_rule(std::shared_ptr<Rule> rule)
  {
    _rules.push_back(rule);
  }

  const std::list<std::shared_ptr<Rule>>& Configuration::rules() const
  {
    return _rules;
  }

#if TEST
  const char* ConfigurationManager::CONFIG_FILE_PATH = "test/res/test_config.json";
#else
  const char* ConfigurationManager::CONFIG_FILE_PATH = "config.json";
#endif

  ConfigurationManager::ConfigurationManager()
  {
    try
    {
      _configuration = parse_configuration();
    }
    catch (std::invalid_argument& e)
    {
      ERROR(e.what());
      throw std::exception();
    }
  }

  std::unique_ptr<Configuration> ConfigurationManager::parse_configuration()
  {
    std::unique_ptr<Configuration> configuration{new Configuration()};

    std::ifstream ifs(CONFIG_FILE_PATH);
    std::string config( (std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()) );

    Json::Reader reader;
    Json::Value config_root;
    if (!reader.parse(config, config_root))
    {
      throw std::invalid_argument("Could not parse configuration");
    }

    if (!config_root.isObject() ||
        !config_root.isMember("configuration"))
    {
      ERROR("The root object must be configuration");
      throw std::invalid_argument("Could not parse configuration");
    }

    const Json::Value config_configuration = config_root["configuration"];

    if (!config_configuration.isObject() ||
        !config_configuration.isMember("rules") ||
        !config_configuration["rules"].isArray())
    {
      ERROR("Could not parse the configuration object");
      throw std::invalid_argument("Could not parse configuration");
    }

    const Json::Value config_rules = config_configuration["rules"];

    for (Json::Value::ArrayIndex i = 0; i != config_rules.size(); i++)
    {
      if (!config_rules[i].isMember("period") ||
          !config_rules[i].isMember("full-backup-period") ||
          !config_rules[i].isMember("strategy") ||
          !config_rules[i].isMember("locations") ||
          !config_rules[i]["locations"].isArray())
      {
        ERROR("Could not parse rule");
        throw std::invalid_argument("Could not parse configuration");
      }

      if (!config_rules[i]["period"].isMember("value") ||
          !config_rules[i]["period"].isMember("unit"))
      {
        ERROR("Could not parse period");
        return nullptr;
      }

      Period period;

      try
      {
        period = Period{config_rules[i]["period"]["value"].asInt(),
                        StringToTimeUnit(config_rules[i]["period"]["unit"].asString())};
      }
      catch (std::invalid_argument& e)
      {
        ERROR(e.what());
        throw std::invalid_argument("Could not parse configuration");
      }

      Period full_backup_period;

      try
      {
        full_backup_period = Period{config_rules[i]["full-backup-period"]["value"].asInt(),
                                    StringToTimeUnit(config_rules[i]["full-backup-period"]["unit"].asString())};
      }
      catch (std::invalid_argument& e)
      {
        ERROR(e.what());
        throw std::invalid_argument("Could not parse configuration");
      }

      STRATEGY strategy;
      
      try
      {
        strategy = StringToStrategy(config_rules[i]["strategy"].asString());
      }
      catch (std::invalid_argument& e)
      {
        ERROR(e.what());
        throw std::invalid_argument("Could not parse configuration");
      }

      std::shared_ptr<Rule> rule{new Rule(period, full_backup_period, strategy)};

      const Json::Value rule_locations = config_rules[i]["locations"];

      for (Json::Value::ArrayIndex j = 0; j != rule_locations.size(); j++)
      {
        if (!rule_locations[j].isMember("original") ||
            !rule_locations[j].isMember("backup"))
        {
          ERROR("Could not parse location");
          throw std::invalid_argument("Could not parse configuration");
        }

        std::string original = rule_locations[j]["original"].asString();
        std::string backup = rule_locations[j]["backup"].asString();

        std::shared_ptr<Location> location{new Location(original, backup)};

        rule->add_location(location);
      }

      configuration->add_rule(rule);
    }

    return configuration;
  }

  ConfigurationManager& ConfigurationManager::instance()
  {
    static ConfigurationManager instance;
    return instance;
  }

  const Configuration* ConfigurationManager::configuration() const
  {
    return _configuration.get();
  }
}
