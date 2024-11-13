#pragma once

#include <memory>
#include <ormpp/dbng.hpp> // Include ormpp header
#include "../config/config.hpp"
#include <ormpp/mysql.hpp>

class DatabaseManager
{
public:
  DatabaseManager()
  {
    // Initialize the MySQL connection pool using ormpp
    pool_.connect(
        Config::AppConfig::getMySQLHost(),
        Config::AppConfig::getMySQLUser(),
        Config::AppConfig::getMySQLPassword(),
        Config::AppConfig::getMySQLDatabase());
  }

  // Get database connection pool
  auto &getDatabase()
  {
    return pool_;
  }

private:
  ormpp::dbng<ormpp::mysql> pool_;
  // Use correct namespace for dbng and mysql
};