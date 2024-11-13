#pragma once
#include "Router.hpp"
#include "../controllers/HelloController.hpp"

namespace Routes
{
  class HelloRouter : public Router
  {
  public:
    void register_routes(crow::App<> &app) override
    {
      CROW_ROUTE(app, "/api/hello")
          .methods("GET"_method)([](const crow::request &req)
                                 { return Controllers::HelloController::hello(); });

      CROW_ROUTE(app, "/api/redis")
          .middlewares<Middlewares::JWTMiddleware>()
          .methods("GET"_method)([](const crow::request &req)
                                 { return Controllers::HelloController::redis(); });
    }
  };
}
