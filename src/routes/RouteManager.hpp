// routes/RouteManager.hpp
#pragma once
#include "Router.hpp"
#include "HelloRouter.hpp"
#include "AuthRouter.hpp"
#include "UserRouter.hpp"
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
  }

  void register_all_routes(crow::App<> &app)
  {
    // Register all routes
    for (const auto &router : routers)
    {
      router->register_routes(app);
    }
  }
};