#pragma once

#include <sw/redis++/redis++.h>
#include "../config/config.hpp"
#include <iostream>
#include <string>

using namespace sw::redis;

class RedisManager
{
public:
  RedisManager()
  {
    try
    {
      // Create the connection URL using string concatenation
      std::string connection_url = std::string("tcp://") +
                                   Config::AppConfig::getRedisHost() + ":" +
                                   std::to_string(Config::AppConfig::getRedisPort());

      // Create connection options
      ConnectionOptions connection_opts;
      connection_opts.host = Config::AppConfig::getRedisHost();
      connection_opts.port = Config::AppConfig::getRedisPort();

      // Optional: Add timeout and other settings
      connection_opts.socket_timeout = std::chrono::milliseconds(200);
      connection_opts.connect_timeout = std::chrono::milliseconds(200);

      // Initialize Redis connection with options
      redis_ = std::make_unique<Redis>(connection_opts);
    }
    catch (const Error &e)
    {
      std::cerr << "Redis connection error: " << e.what() << std::endl;
      throw;
    }
  }

  // Prevent copying
  RedisManager(const RedisManager &) = delete;
  RedisManager &operator=(const RedisManager &) = delete;

  // Allow moving
  RedisManager(RedisManager &&) = default;
  RedisManager &operator=(RedisManager &&) = default;

  // Destructor
  ~RedisManager() = default;

  // Getter for Redis connection
  Redis *getRedis()
  {
    if (!redis_)
    {
      throw std::runtime_error("Redis connection not initialized");
    }
    return redis_.get();
  }

private:
  std::unique_ptr<Redis> redis_;
};