#pragma once
#include "Router.hpp"
#include "../controllers/HelloController.hpp"

namespace Routes
{
  class HelloRouter : public Router
  {
  public:
    void register_routes(crow::App<Middlewares::JWTMiddleware> &app) override
    {
      CROW_ROUTE(app, "/api/hello")
          .methods("GET"_method)([](const crow::request &req)
                                 { return Controllers::HelloController::hello(); });

      CROW_ROUTE(app, "/api/redis").methods("GET"_method)([](const crow::request &req)
                                                          { return Controllers::HelloController::redis(); });

      CROW_ROUTE(app, "/api/sendMessage").methods("GET"_method)([](const crow::request &req)
                                                                { return Controllers::HelloController::sendMessage(); });
    }
  };
}
