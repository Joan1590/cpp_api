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

        // Query user by email
        char buffer[256];
        sprintf(buffer, "email='%s'", email.c_str());
        auto result = db.query<User>(buffer);

        if (result.empty())
        {
          return unauthorized("Invalid email or password");
        }

        // Verify password
        if (!verifyPassword(email, password))
        {
          return unauthorized("Invalid email or password");
        }

        // Generate JWT token
        const char *jwt_secret = Config::AppConfig::getJWTSecret();

        printf("JWT secret: %s\n", jwt_secret);

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
    static bool verifyPassword(const std::string &email, const std::string &password)
    {
      try
      {
        DatabaseManager dbManager;
        auto &db = dbManager.getDatabase();

        char buffer[256];
        sprintf(buffer, "email='%s'", email.c_str());
        auto result = db.query<User>(buffer);

        if (result.empty())
        {
          return false;
        }

        // Compare hashed passwords
        return result[0].password == HashUtils::sha256(password);
      }
      catch (const std::exception &e)
      {
        return false;
      }
    }
  };
}