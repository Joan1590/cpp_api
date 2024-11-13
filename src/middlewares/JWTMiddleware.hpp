#pragma once
#include "crow.h"
#include "jwt-cpp/jwt.h"
#include <string>
#include "../config/config.hpp"

namespace Middlewares
{
  // middlewares/JWTMiddleware.hpp
  class JWTMiddleware : crow::ILocalMiddleware
  {
  public:
    struct context
    {
    };
    std::vector<std::string> public_routes;

    void before_handle(crow::request &req, crow::response &res, context &ctx)
    {
      try
      {
        // Your existing JWT verification code
        std::string auth_header = req.get_header_value("Authorization");
        if (auth_header.empty() || auth_header.substr(0, 7) != "Bearer ")
        {
          res = crow::response(401);
          res.write("Missing or invalid Authorization header");
          res.end();
          return;
        }
        // ... rest of your JWT verification code
      }
      catch (const std::exception &e)
      {
        res = crow::response(401);
        res.write("Invalid token");
        res.end();
      }
    }

    void after_handle(crow::request &, crow::response &, context &) {}
  };
}