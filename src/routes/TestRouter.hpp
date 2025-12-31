#pragma once
#include "Router.hpp"
#include "../controllers/TestController.hpp"

namespace Routes
{
  class TestRouter : public Router
  {
  public:
    void register_routes(crow::App<Middlewares::JWTMiddleware> &app) override
    {
      // Run all tests
      CROW_ROUTE(app, "/api/tests/run")
          .methods("GET"_method)([](const crow::request &req)
                                 { return Controllers::TestController::runTests(req); });

      // Run specific test suite
      CROW_ROUTE(app, "/api/tests/run/<string>")
          .methods("GET"_method)([](const crow::request &req, std::string suiteName)
                                 { return Controllers::TestController::runSuite(req, suiteName); });
    }
  };
}
