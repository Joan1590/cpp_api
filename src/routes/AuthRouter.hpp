#pragma once

#include "Router.hpp"
#include "../controllers/AuthController.hpp"
#include "../middlewares/JWTMiddleware.hpp"
#include "../middlewares/NoMiddleware.hpp"

namespace Routes
{
  class AuthRouter : public Router
  {
  public:
    void register_routes(crow::App<> &app) override
    {
      CROW_ROUTE(app, "/api/auth/login")
          .methods("POST"_method)([](const crow::request &req)
                                  { return Controllers::AuthController::login(req); });
    }
  };
}