#include "routes/RouteManager.hpp"
#include "database/MigrationManager.hpp"
#include "middlewares/JWTMiddleware.hpp"

int main(int argc, char *argv[])
{
  if (argc > 1 && strcmp(argv[1], "-migrate") == 0)
  {
    MigrationManager manager;
    manager.runMigrations();
    return 0;
  }

  auto &app = Router::getApp();

  // Register all routes using RouteManager
  RouteManager routeManager;
  routeManager.register_all_routes(app);

  app.port(Config::AppConfig::getPort())
      .multithreaded()
      .run();

  // auto app2 = crow::SimpleApp();                                                 // or crow::App()

  // CROW_ROUTE(app2, "/with_middleware").CROW_MIDDLEWARES(app2, JWTMiddleware) // Can be used more than one
  //     * ([]() {                                                                 // middleware.
  //         *return "Hello world!";
  //         *
  //       });

  // CROW_ROUTE(app, "/")
  //     .CROW_MIDDLEWARES(app, Middlewares::JWTMiddleware)
  //     .methods("GET"_method)([](const crow::request &req)
  //                            { return "Hello world!"; });

  return 0;
}