// routes/Router.hpp
#pragma once
#include "crow.h"
#include "../middlewares/JWTMiddleware.hpp"

class Router
{
public:
  static crow::App<> &getApp()
  {
    static crow::App<> app;
    return app;
  }

  virtual void register_routes(crow::App<> &app) = 0;
  virtual ~Router() = default;
};