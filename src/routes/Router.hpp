// routes/Router.hpp
#pragma once
#include "crow.h"
#include "../middlewares/JWTMiddleware.hpp"

class Router
{
public:
  static crow::App<Middlewares::JWTMiddleware> &getApp()
  {
    static crow::App<Middlewares::JWTMiddleware> app;
    return app;
  }

  virtual void register_routes(crow::App<Middlewares::JWTMiddleware> &app) = 0;
  virtual ~Router() = default;
};