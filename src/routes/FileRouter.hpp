#pragma once
#include "Router.hpp"
#include "../controllers/FileController.hpp"

namespace Routes
{
  class FileRouter : public Router
  {
  public:
    void register_routes(crow::App<Middlewares::JWTMiddleware> &app) override
    {
      // Upload file
      CROW_ROUTE(app, "/api/files/upload")
          .methods("POST"_method)([](const crow::request &req)
                                  { return Controllers::FileController::upload(req); });

      // List all files
      CROW_ROUTE(app, "/api/files")
          .methods("GET"_method)([](const crow::request &req)
                                 { return Controllers::FileController::listFiles(req); });

      // Get file info
      CROW_ROUTE(app, "/api/files/<string>/info")
          .methods("GET"_method)([](const crow::request &req, std::string filename)
                                 { return Controllers::FileController::fileInfo(req, filename); });

      // Download file
      CROW_ROUTE(app, "/api/files/<string>/download")
          .methods("GET"_method)([](const crow::request &req, std::string filename)
                                 { return Controllers::FileController::download(req, filename); });

      // View/serve file inline
      CROW_ROUTE(app, "/api/files/<string>/view")
          .methods("GET"_method)([](const crow::request &req, std::string filename)
                                 { return Controllers::FileController::view(req, filename); });

      // Delete file
      CROW_ROUTE(app, "/api/files/<string>")
          .CROW_MIDDLEWARES(app, Middlewares::JWTMiddleware)
          .methods("DELETE"_method)([](const crow::request &req, std::string filename)
                                    { return Controllers::FileController::deleteFile(req, filename); });
    }
  };
}
