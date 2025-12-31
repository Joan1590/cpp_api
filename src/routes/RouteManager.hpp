// routes/RouteManager.hpp
#pragma once
#include "Router.hpp"
#include "HelloRouter.hpp"
#include "AuthRouter.hpp"
#include "UserRouter.hpp"
#include "UtilsRouter.hpp"
#include "ValidationRouter.hpp"
#include "ExportRouter.hpp"
#include "I18nRouter.hpp"
#include "FileRouter.hpp"
#include "TestRouter.hpp"
#include "WebSocketRouter.hpp"
#include <memory>
#include <vector>

class RouteManager
{
private:
  std::vector<std::unique_ptr<Router>> routers;

public:
  RouteManager()
  {
    // Add all routers
    routers.push_back(std::make_unique<Routes::HelloRouter>());
    routers.push_back(std::make_unique<Routes::AuthRouter>());
    routers.push_back(std::make_unique<Routes::UserRouter>());
    routers.push_back(std::make_unique<Routes::UtilsRouter>());
    routers.push_back(std::make_unique<Routes::ValidationRouter>());
    routers.push_back(std::make_unique<Routes::ExportRouter>());
    routers.push_back(std::make_unique<Routes::I18nRouter>());
    routers.push_back(std::make_unique<Routes::FileRouter>());
    routers.push_back(std::make_unique<Routes::TestRouter>());
    routers.push_back(std::make_unique<Routes::WebSocketRouter>());
  }

  void register_all_routes(crow::App<Middlewares::JWTMiddleware> &app)
  {
    // Register all routes
    for (const auto &router : routers)
    {
      router->register_routes(app);
    }
  }
};