#pragma once
#include "crow.h"
#include <nlohmann/json.hpp>

namespace Controllers
{
  class BaseController
  {
  protected:
    using json = nlohmann::json;

    // Response helpers
    static crow::response ok(const json &data)
    {
      return json_response(200, data);
    }

    static crow::response created(const json &data)
    {
      return json_response(201, data);
    }

    static crow::response bad_request(const std::string &message)
    {
      return error_response(400, message);
    }

    static crow::response not_found(const std::string &message)
    {
      return error_response(404, message);
    }

    static crow::response server_error(const std::string &message)
    {
      return error_response(500, message);
    }

    static crow::response unauthorized(const std::string &message)
    {
      return error_response(401, message);
    }

    // Request helpers
    static json parse_body(const crow::request &req)
    {
      try
      {
        return json::parse(req.body);
      }
      catch (const std::exception &e)
      {
        throw std::runtime_error("Invalid JSON body");
      }
    }

  private:
    static crow::response json_response(int status, const json &data)
    {
      auto res = crow::response(status, data.dump());
      res.add_header("Content-Type", "application/json; charset=utf-8");
      return res;
    }

    static crow::response error_response(int status, const std::string &message)
    {
      json error = {
          {"error", message}};
      return json_response(status, error);
    }
  };
}