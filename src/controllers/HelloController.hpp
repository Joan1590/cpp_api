#pragma once
#include "crow.h"
#include <nlohmann/json.hpp>
#include "BaseController.hpp"
#include "../redis/RedisManager.hpp"

using json = nlohmann::json;

namespace Controllers
{
  class HelloController : public BaseController
  {
  public:
    static crow::response hello()
    {
      json response = {
          {"message", "Hello World!"},
          {"status", "success"}};
      return ok(response);
    }

    // example of redis usage
    static crow::response redis()
    {
      RedisManager redis_manager;

      // Use the Redis connection
      auto redis = redis_manager.getRedis();

      // Example: Set a value
      redis->set("test_key", "test_value");

      // Example: Get a value
      auto value = redis->get("test_key");

      // Example: Increment a value
      // int value = redis->incr("counter");

      // json response = {
      //     {"result", std::string("The counter is: ") + std::to_string(value)},
      //     {"status", "success"}};

      json response = {
          {"result", value.value()},
          {"status", "success"}};
      return ok(response);
    }
  };
}