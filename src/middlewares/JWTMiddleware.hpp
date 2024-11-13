#pragma once
#include "crow.h"
#include "jwt-cpp/jwt.h"
#include <string>
#include "../config/config.hpp"
#include "../controllers/BaseController.hpp"

namespace Middlewares
{
  class JWTMiddleware : public crow::ILocalMiddleware, public Controllers::BaseController
  {
  public:
    struct context
    {
    };

    void before_handle(crow::request &req, crow::response &res, context &ctx)
    {
      try
      {
        // Get token from Authorization header
        std::string auth_header = req.get_header_value("Authorization");
        if (auth_header.empty() || auth_header.substr(0, 7) != "Bearer ") // Added empty check
        {
          res = unauthorized("Missing or invalid Authorization header");
          res.end();
          return;
        }

        std::string token = auth_header.substr(7);

        // Verify token
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
                            .allow_algorithm(jwt::algorithm::hs256(Config::AppConfig::getJWTSecret()));

        verifier.verify(decoded);
      }
      catch (const std::exception &e)
      {
        res = unauthorized("Invalid token");
        res.end();
      }
    }

    void after_handle(crow::request & /*req*/, crow::response & /*res*/, context & /*ctx*/)
    {
      // Optional post-processing
    }
  };
}
