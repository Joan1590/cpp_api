#pragma once
#include "crow.h"
#include <nlohmann/json.hpp>
#include "../database/DatabaseManager.hpp"
#include "../models/User.hpp"
#include "BaseController.hpp"
#include "../utils/HashUtils.hpp"

namespace Controllers
{
  class UserController : public BaseController
  {
  public:
    static crow::response get()
    {
      try
      {
        DatabaseManager dbManager;
        auto &db = dbManager.getDatabase();
        auto result = db.query<User>();

        if (result.empty())
        {
          return not_found("No users found in the database");
        }

        json response = json::array();
        for (auto &user : result)
        {
          response.push_back({{"id", user.id},
                              {"name", user.name},
                              {"email", user.email}});
        }

        return ok(response);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    static crow::response getOne(int id)
    {
      try
      {
        DatabaseManager dbManager;
        auto &db = dbManager.getDatabase();

        char buffer[100];
        sprintf(buffer, "id=%d", id);
        auto result = db.query<User>(buffer);

        if (result.empty())
        {
          return not_found("User not found");
        }

        json response = {
            {"id", result[0].id},
            {"name", result[0].name},
            {"email", result[0].email}};

        return ok(response);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    static crow::response create(const crow::request &req)
    {
      try
      {
        auto jsonData = parse_body(req);

        DatabaseManager dbManager;
        auto &db = dbManager.getDatabase();

        auto user = User();
        user.name = jsonData["name"].get<std::string>();
        user.email = jsonData["email"].get<std::string>();
        // Hash the password before storing
        user.password = HashUtils::sha256(jsonData["password"].get<std::string>());

        db.insert(user);

        // Get the last inserted user
        auto result = db.query<User>("id = LAST_INSERT_ID()");

        json response = {
            {"message", "User created successfully"},
            {"user", {{"id", result[0].id}, {"name", result[0].name}, {"email", result[0].email}}}};

        return created(response);
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

    static crow::response update(int id, const crow::request &req)
    {
      try
      {
        auto jsonData = parse_body(req);

        DatabaseManager dbManager;
        auto &db = dbManager.getDatabase();

        char buffer[100];
        sprintf(buffer, "id=%d", id);
        auto result = db.query<User>(buffer);

        if (result.empty())
        {
          return not_found("User not found");
        }

        result[0].name = jsonData["name"].get<std::string>();
        result[0].email = jsonData["email"].get<std::string>();
        // Hash the password before storing
        result[0].password = HashUtils::sha256(jsonData["password"].get<std::string>());

        db.update(result[0]);

        json response = {
            {"message", "User updated successfully"},
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

    static crow::response deleteOne(int id)
    {
      try
      {
        DatabaseManager dbManager;
        auto &db = dbManager.getDatabase();

        char buffer[100];
        sprintf(buffer, "id=%d", id);
        auto result = db.query<User>(buffer);

        if (result.empty())
        {
          return not_found("User not found");
        }

        db.delete_records<User>(buffer);

        return ok({"message", "User deleted successfully"});
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }
  };
} // namespace Controllers