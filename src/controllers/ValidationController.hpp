#pragma once
#include "crow.h"
#include "BaseController.hpp"
#include "../validation/Validator.hpp"

namespace Controllers
{
  class ValidationController : public BaseController
  {
  public:
    // Validate user registration data
    static crow::response validateUser(const crow::request &req)
    {
      try
      {
        auto body = parse_body(req);

        auto validator = Validation::make(body)
                             .required("name")
                             .string("name")
                             .minLength("name", 2)
                             .maxLength("name", 100)
                             .required("email")
                             .email("email")
                             .required("password")
                             .minLength("password", 8)
                             .confirmed("password")
                             .required("age")
                             .integer("age")
                             .min("age", 18)
                             .max("age", 120);

        auto result = validator.validate();

        if (!result.valid)
        {
          return bad_request(result.toJson().dump());
        }

        return ok({{"message", "Validation passed"}, {"valid", true}});
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Validate product data
    static crow::response validateProduct(const crow::request &req)
    {
      try
      {
        auto body = parse_body(req);

        auto validator = Validation::make(body)
                             .required("name")
                             .string("name")
                             .minLength("name", 3)
                             .required("price")
                             .number("price")
                             .min("price", 0.01)
                             .required("category")
                             .in<std::string>("category", {"electronics", "clothing", "food", "books", "other"})
                             .url("website")
                             .date("release_date");

        auto result = validator.validate();

        json response = {
            {"valid", result.valid},
            {"errors", result.toJson()["errors"]}};

        if (!result.valid)
        {
          auto res = crow::response(422, response.dump());
          res.add_header("Content-Type", "application/json");
          return res;
        }

        return ok(response);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Custom validation example
    static crow::response validateCustom(const crow::request &req)
    {
      try
      {
        auto body = parse_body(req);

        // Custom validator to check if username contains no spaces
        auto noSpaces = [](const json &value) -> bool
        {
          if (!value.is_string())
            return true;
          return value.get<std::string>().find(' ') == std::string::npos;
        };

        auto validator = Validation::make(body)
                             .required("username")
                             .custom("username", noSpaces, "Username cannot contain spaces")
                             .pattern("phone", R"(\+?[0-9]{10,15})", "Invalid phone number format");

        auto result = validator.validate();

        return ok(result.toJson());
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }
  };
}
