#pragma once
#include "Router.hpp"
#include "../controllers/ValidationController.hpp"

namespace Routes
{
  class ValidationRouter : public Router
  {
  public:
    void register_routes(crow::App<Middlewares::JWTMiddleware> &app) override
    {
      // Validate user registration
      CROW_ROUTE(app, "/api/validate/user")
          .methods("POST"_method)([](const crow::request &req)
                                  { return Controllers::ValidationController::validateUser(req); });

      // Validate product
      CROW_ROUTE(app, "/api/validate/product")
          .methods("POST"_method)([](const crow::request &req)
                                  { return Controllers::ValidationController::validateProduct(req); });

      // Custom validation
      CROW_ROUTE(app, "/api/validate/custom")
          .methods("POST"_method)([](const crow::request &req)
                                  { return Controllers::ValidationController::validateCustom(req); });
    }
  };
}
