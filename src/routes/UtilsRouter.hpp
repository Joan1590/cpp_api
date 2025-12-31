#pragma once
#include "Router.hpp"
#include "../controllers/UtilsController.hpp"

namespace Routes
{
  class UtilsRouter : public Router
  {
  public:
    void register_routes(crow::App<Middlewares::JWTMiddleware> &app) override
    {
      // String utilities
      CROW_ROUTE(app, "/api/utils/string")
          .methods("POST"_method)([](const crow::request &req)
                                  { return Controllers::UtilsController::stringUtils(req); });

      // Date utilities
      CROW_ROUTE(app, "/api/utils/date")
          .methods("GET"_method)([](const crow::request &req)
                                 { return Controllers::UtilsController::dateUtils(req); });

      // Generate random values
      CROW_ROUTE(app, "/api/utils/random")
          .methods("GET"_method)([](const crow::request &req)
                                 { return Controllers::UtilsController::generateRandom(req); });

      // JSON utilities
      CROW_ROUTE(app, "/api/utils/json")
          .methods("POST"_method)([](const crow::request &req)
                                  { return Controllers::UtilsController::jsonUtils(req); });
    }
  };
}
