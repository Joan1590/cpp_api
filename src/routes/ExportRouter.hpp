#pragma once
#include "Router.hpp"
#include "../controllers/ExportController.hpp"

namespace Routes
{
  class ExportRouter : public Router
  {
  public:
    void register_routes(crow::App<Middlewares::JWTMiddleware> &app) override
    {
      // Export users to CSV
      CROW_ROUTE(app, "/api/export/users/csv")
          .methods("GET"_method)([](const crow::request &req)
                                 { return Controllers::ExportController::exportUsersCSV(req); });

      // Export users to XML
      CROW_ROUTE(app, "/api/export/users/xml")
          .methods("GET"_method)([](const crow::request &req)
                                 { return Controllers::ExportController::exportUsersXML(req); });

      // Export custom data to CSV
      CROW_ROUTE(app, "/api/export/csv")
          .methods("POST"_method)([](const crow::request &req)
                                  { return Controllers::ExportController::exportToCSV(req); });

      // Export custom data to HTML table
      CROW_ROUTE(app, "/api/export/html")
          .methods("POST"_method)([](const crow::request &req)
                                  { return Controllers::ExportController::exportToHTML(req); });

      // Export custom data to text table
      CROW_ROUTE(app, "/api/export/text")
          .methods("POST"_method)([](const crow::request &req)
                                  { return Controllers::ExportController::exportToTextTable(req); });
    }
  };
}
