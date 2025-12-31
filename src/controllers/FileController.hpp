#pragma once
#include "crow.h"
#include "BaseController.hpp"
#include "../files/FileHandler.hpp"

namespace Controllers
{
  class FileController : public BaseController
  {
  private:
    static Files::FileHandler &getHandler()
    {
      static Files::FileHandler handler("./uploads", 10 * 1024 * 1024); // 10MB max
      return handler;
    }

  public:
    // Upload a file
    static crow::response upload(const crow::request &req)
    {
      try
      {
        // Get content type
        std::string contentType = req.get_header_value("Content-Type");

        if (contentType.find("multipart/form-data") == std::string::npos)
        {
          // Handle raw file upload with filename in header
          std::string filename = req.get_header_value("X-File-Name");
          if (filename.empty())
          {
            filename = "uploaded_file";
          }

          auto result = getHandler().saveFile(req.body, filename);

          if (!result)
          {
            return bad_request("Failed to save file. Check file size and extension.");
          }

          json response = {
              {"message", "File uploaded successfully"},
              {"file", {{"original_name", result->originalName}, {"stored_name", result->storedName}, {"path", result->path}, {"extension", result->extension}, {"mime_type", result->mimeType}, {"size", result->size}}}};

          return created(response);
        }

        // For multipart, parse the body
        crow::multipart::message msg(req);

        if (msg.parts.empty())
        {
          return bad_request("No file provided");
        }

        json uploadedFiles = json::array();

        for (const auto &part : msg.parts)
        {
          std::string filename = "uploaded_file";

          // Try to get filename from Content-Disposition header
          for (const auto &header : part.headers)
          {
            if (header.second.value.find("filename=") != std::string::npos)
            {
              size_t start = header.second.value.find("filename=\"") + 10;
              size_t end = header.second.value.find("\"", start);
              if (end != std::string::npos)
              {
                filename = header.second.value.substr(start, end - start);
              }
            }
          }

          auto result = getHandler().saveFile(part.body, filename);

          if (result)
          {
            uploadedFiles.push_back({{"original_name", result->originalName},
                                     {"stored_name", result->storedName},
                                     {"path", result->path},
                                     {"extension", result->extension},
                                     {"mime_type", result->mimeType},
                                     {"size", result->size}});
          }
        }

        if (uploadedFiles.empty())
        {
          return bad_request("Failed to upload any files");
        }

        json response = {
            {"message", "Files uploaded successfully"},
            {"files", uploadedFiles}};

        return created(response);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Download a file
    static crow::response download(const crow::request &req, const std::string &filename)
    {
      try
      {
        std::string filepath = "./uploads/" + filename;

        if (!Files::FileHandler::exists(filepath))
        {
          return not_found("File not found");
        }

        return Files::FileHandler::downloadResponse(filepath);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // View/serve a file inline
    static crow::response view(const crow::request &req, const std::string &filename)
    {
      try
      {
        std::string filepath = "./uploads/" + filename;

        if (!Files::FileHandler::exists(filepath))
        {
          return not_found("File not found");
        }

        return Files::FileHandler::serveFile(filepath);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Delete a file
    static crow::response deleteFile(const crow::request &req, const std::string &filename)
    {
      try
      {
        std::string filepath = "./uploads/" + filename;

        if (!Files::FileHandler::exists(filepath))
        {
          return not_found("File not found");
        }

        if (!Files::FileHandler::deleteFile(filepath))
        {
          return server_error("Failed to delete file");
        }

        return ok({{"message", "File deleted successfully"}});
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // List all files
    static crow::response listFiles(const crow::request &req)
    {
      try
      {
        auto files = Files::FileHandler::listDirectory("./uploads");

        json fileList = json::array();
        for (const auto &file : files)
        {
          if (!file.isDirectory)
          {
            fileList.push_back({{"name", file.name},
                                {"path", file.path},
                                {"extension", file.extension},
                                {"size", file.size},
                                {"modified_at", file.modifiedAt}});
          }
        }

        json response = {
            {"count", fileList.size()},
            {"files", fileList}};

        return ok(response);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Get file info
    static crow::response fileInfo(const crow::request &req, const std::string &filename)
    {
      try
      {
        std::string filepath = "./uploads/" + filename;

        auto info = Files::FileHandler::getFileInfo(filepath);

        if (!info)
        {
          return not_found("File not found");
        }

        json response = {
            {"name", info->name},
            {"path", info->path},
            {"extension", info->extension},
            {"size", info->size},
            {"modified_at", info->modifiedAt},
            {"is_directory", info->isDirectory}};

        return ok(response);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }
  };
}
