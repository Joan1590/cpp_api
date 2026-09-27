#pragma once
#include "crow.h"
#include <nlohmann/json.hpp>
#include "../database/DatabaseManager.hpp"
#include "../models/User.hpp"
#include "../utils/HashUtils.hpp"
#include "BaseController.hpp"
#include "jwt-cpp/jwt.h"
#include "../config/config.hpp"

namespace Controllers
{
  class AuthController : public BaseController
  {
  public:
    static crow::response login(const crow::request &req)
    {
      try
      {
        auto jsonData = parse_body(req);

        std::string email = jsonData["email"].get<std::string>();
        std::string password = jsonData["password"].get<std::string>();

        DatabaseManager dbManager;
        auto &db = dbManager.getDatabase();

        // Query user by email (prepared statement)
        auto result = db.query_s<User>("email=?", email);

        if (result.empty())
        {
          // Hash anyway so response time does not reveal whether the email exists
          HashUtils::verifyPassword(password, dummyHash());
          return unauthorized("Invalid email or password");
        }

        if (!HashUtils::verifyPassword(password, result[0].password))
        {
          return unauthorized("Invalid email or password");
        }

        // Upgrade legacy unsalted hashes on successful login
        if (HashUtils::needsRehash(result[0].password))
        {
          result[0].password = HashUtils::hashPassword(password);
          db.update(result[0]);
        }

        // Generate JWT token
        const char *jwt_secret = Config::AppConfig::getJWTSecret();

        if (!jwt_secret)
        {
          return server_error("JWT secret not configured");
        }

        auto token = jwt::create()
                         .set_issuer("auth")
                         .set_type("JWS")
                         .set_issued_at(std::chrono::system_clock::now())
                         .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours{24})
                         .set_payload_claim("user_id", jwt::claim(std::to_string(result[0].id)))
                         .set_payload_claim("email", jwt::claim(result[0].email))
                         .sign(jwt::algorithm::hs256{jwt_secret});

        json response = {
            {"message", "Login successful"},
            {"token", token},
            {"user", {{"id", result[0].id}, {"name", result[0].name}, {"email", result[0].email}}}};

        return ok(response);
      }
      catch (const std::runtime_error &e)
      {
        return bad_request(e.what());
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

  private:
    static const std::string &dummyHash()
    {
      static const std::string hash = HashUtils::hashPassword("dummy-password");
      return hash;
    }
  };
}