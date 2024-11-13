#pragma once
#include <cstdlib>
#include <string>

namespace Config
{
  struct AppConfig
  {
    static constexpr const char *VERSION = "1.0.0";

    static const char *getEnvVar(const std::string &key)
    {
      const char *val = std::getenv(key.c_str());
      val == nullptr ? std::string() : std::string(val);
      return val;
    }

    static const char *getMySQLHost()
    {
      return getEnvVar("MYSQL_HOST");
    }

    static const char *getMySQLUser()
    {
      return getEnvVar("MYSQL_USER");
    }

    static const char *getMySQLPassword()
    {
      return getEnvVar("MYSQL_PASSWORD");
    }

    static const char *getMySQLDatabase()
    {
      return getEnvVar("MYSQL_DATABASE");
    }

    static int getMySQLPort()
    {
      return 3306;
    }

    static int getPort()
    {
      return getEnvVar("PORT") ? std::stoi(getEnvVar("PORT")) : 3000;
    }

    static const char *getRedisHost()
    {
      return getEnvVar("REDIS_HOST");
    }

    static int getRedisPort()
    {
      return getEnvVar("REDIS_PORT") ? std::stoi(getEnvVar("REDIS_PORT")) : 6379;
    }

    static const char *getJWTSecret()
    {
      return getEnvVar("JWT_SECRET");
    }
  };
}