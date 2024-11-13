#pragma once
#include "Router.hpp"
#include "../controllers/UserController.hpp"
#include "../middlewares/JWTMiddleware.hpp"

namespace Routes
{
  class UserRouter : public Router
  {
  public:
    void register_routes(crow::App<> &app) override
    {
      CROW_ROUTE(app, "/api/users")
          .methods("GET"_method)([](const crow::request &req)
                                 { return Controllers::UserController::get(); });

      CROW_ROUTE(app, "/api/users/<string>")
          .methods("GET"_method)([](const crow::request &req, std::string id)
                                 { return Controllers::UserController::getOne(std::stoi(id)); });

      CROW_ROUTE(app, "/api/users")
          .methods("POST"_method)([](const crow::request &req)
                                  { return Controllers::UserController::create(req); });

      CROW_ROUTE(app, "/api/users/<string>")
          .middlewares<Middlewares::JWTMiddleware>()
          .methods("PUT"_method)([](const crow::request &req, std::string id)
                                 { return Controllers::UserController::update(std::stoi(id), req); });

      CROW_ROUTE(app, "/api/users/<string>")
          .middlewares<Middlewares::JWTMiddleware>()
          .methods("DELETE"_method)([](const crow::request &req, std::string id)
                                    { return Controllers::UserController::deleteOne(std::stoi(id)); });
    }
  };
}