#include "routes/RouteManager.hpp"
#include "database/MigrationManager.hpp"
#include "middlewares/JWTMiddleware.hpp"
#include "events/EventManager.hpp"

int main(int argc, char *argv[])
{
  if (argc > 1 && strcmp(argv[1], "-migrate") == 0)
  {
    MigrationManager manager;
    manager.runMigrations();
    return 0;
  }

  auto &app = Router::getApp();

  // Run threads for EventManager
  Events::EventManager eventManager;
  eventManager.runThreads();

  // Register all routes using RouteManager
  RouteManager routeManager;
  routeManager.register_all_routes(app);

  app.port(Config::AppConfig::getPort())
      .multithreaded()
      .run();

  return 0;
}