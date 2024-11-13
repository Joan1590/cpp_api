#pragma once
#include "crow.h"
#include "jwt-cpp/jwt.h"
#include <string>
#include "../config/config.hpp"

namespace Middlewares
{
  struct NoMiddleware
  {
    struct context
    {
    };

    void before_handle(crow::request &, crow::response &, context &) {}
    void after_handle(crow::request &, crow::response &, context &) {}
  };
}