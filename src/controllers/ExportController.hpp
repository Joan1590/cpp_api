#pragma once
#include "crow.h"
#include "BaseController.hpp"
#include "../export/DataExporter.hpp"
#include "../database/DatabaseManager.hpp"
#include "../models/User.hpp"

namespace Controllers
{
  class ExportController : public BaseController
  {
  public:
    // Export users to CSV
    static crow::response exportUsersCSV(const crow::request &req)
    {
      try
      {
        DatabaseManager dbManager;
        auto &db = dbManager.getDatabase();
        auto users = db.query<User>();

        json data = json::array();
        for (const auto &user : users)
        {
          data.push_back({{"id", user.id},
                          {"name", user.name},
                          {"email", user.email}});
        }

        return Export::DataExporter::csvResponse(data, "users.csv");
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Export users to XML
    static crow::response exportUsersXML(const crow::request &req)
    {
      try
      {
        DatabaseManager dbManager;
        auto &db = dbManager.getDatabase();
        auto users = db.query<User>();

        json data = json::array();
        for (const auto &user : users)
        {
          data.push_back({{"id", user.id},
                          {"name", user.name},
                          {"email", user.email}});
        }

        return Export::DataExporter::xmlResponse(data, "users.xml");
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Export custom data to CSV (from request body)
    static crow::response exportToCSV(const crow::request &req)
    {
      try
      {
        auto body = parse_body(req);

        if (!body.contains("data") || !body["data"].is_array())
        {
          return bad_request("Request must contain 'data' array");
        }

        std::vector<std::string> columns;
        if (body.contains("columns") && body["columns"].is_array())
        {
          for (const auto &col : body["columns"])
          {
            columns.push_back(col.get<std::string>());
          }
        }

        std::string filename = "export.csv";
        if (body.contains("filename") && body["filename"].is_string())
        {
          filename = body["filename"].get<std::string>();
        }

        std::string csv = Export::DataExporter::toCSV(body["data"], columns);

        crow::response res(200, csv);
        res.add_header("Content-Type", "text/csv; charset=utf-8");
        res.add_header("Content-Disposition", "attachment; filename=\"" + filename + "\"");
        return res;
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Export to HTML table format
    static crow::response exportToHTML(const crow::request &req)
    {
      try
      {
        auto body = parse_body(req);

        if (!body.contains("data") || !body["data"].is_array())
        {
          return bad_request("Request must contain 'data' array");
        }

        std::string html = Export::DataExporter::toHTML(body["data"]);

        crow::response res(200, html);
        res.add_header("Content-Type", "text/html; charset=utf-8");
        return res;
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Export to text table format (ASCII art table)
    static crow::response exportToTextTable(const crow::request &req)
    {
      try
      {
        auto body = parse_body(req);

        if (!body.contains("data") || !body["data"].is_array())
        {
          return bad_request("Request must contain 'data' array");
        }

        std::string table = Export::DataExporter::toTextTable(body["data"]);

        crow::response res(200, table);
        res.add_header("Content-Type", "text/plain; charset=utf-8");
        return res;
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }
  };
}
