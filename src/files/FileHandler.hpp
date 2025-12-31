#pragma once
#include "crow.h"
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <random>
#include <sstream>
#include <iomanip>
#include <chrono>

namespace Files
{
  namespace fs = std::filesystem;

  struct UploadedFile
  {
    std::string originalName;
    std::string storedName;
    std::string path;
    std::string extension;
    std::string mimeType;
    size_t size;
  };

  struct FileInfo
  {
    std::string name;
    std::string path;
    std::string extension;
    size_t size;
    std::string modifiedAt;
    bool isDirectory;
  };

  class FileHandler
  {
  private:
    std::string uploadDir;
    size_t maxFileSize;
    std::vector<std::string> allowedExtensions;

    static std::string generateUniqueFilename(const std::string &extension)
    {
      auto now = std::chrono::system_clock::now();
      auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> dis(1000, 9999);

      std::stringstream ss;
      ss << timestamp << "_" << dis(gen);
      if (!extension.empty())
      {
        ss << "." << extension;
      }
      return ss.str();
    }

    static std::string getExtension(const std::string &filename)
    {
      size_t pos = filename.rfind('.');
      if (pos == std::string::npos)
        return "";
      return filename.substr(pos + 1);
    }

    static std::string getMimeType(const std::string &extension)
    {
      static const std::map<std::string, std::string> mimeTypes = {
          {"txt", "text/plain"},
          {"html", "text/html"},
          {"css", "text/css"},
          {"js", "application/javascript"},
          {"json", "application/json"},
          {"xml", "application/xml"},
          {"pdf", "application/pdf"},
          {"zip", "application/zip"},
          {"gz", "application/gzip"},
          {"tar", "application/x-tar"},
          {"png", "image/png"},
          {"jpg", "image/jpeg"},
          {"jpeg", "image/jpeg"},
          {"gif", "image/gif"},
          {"svg", "image/svg+xml"},
          {"webp", "image/webp"},
          {"ico", "image/x-icon"},
          {"mp3", "audio/mpeg"},
          {"wav", "audio/wav"},
          {"mp4", "video/mp4"},
          {"webm", "video/webm"},
          {"csv", "text/csv"},
          {"doc", "application/msword"},
          {"docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
          {"xls", "application/vnd.ms-excel"},
          {"xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"}};

      auto it = mimeTypes.find(extension);
      return it != mimeTypes.end() ? it->second : "application/octet-stream";
    }

  public:
    FileHandler(const std::string &uploadDirectory = "./uploads", size_t maxSize = 10 * 1024 * 1024)
        : uploadDir(uploadDirectory), maxFileSize(maxSize)
    {
      ensureDirectoryExists(uploadDir);
    }

    // Set allowed file extensions
    void setAllowedExtensions(const std::vector<std::string> &extensions)
    {
      allowedExtensions = extensions;
    }

    // Set maximum file size
    void setMaxFileSize(size_t size)
    {
      maxFileSize = size;
    }

    // Ensure directory exists
    static bool ensureDirectoryExists(const std::string &path)
    {
      try
      {
        if (!fs::exists(path))
        {
          return fs::create_directories(path);
        }
        return true;
      }
      catch (...)
      {
        return false;
      }
    }

    // Check if extension is allowed
    bool isExtensionAllowed(const std::string &extension) const
    {
      if (allowedExtensions.empty())
        return true;
      std::string ext = extension;
      std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
      for (const auto &allowed : allowedExtensions)
      {
        if (ext == allowed)
          return true;
      }
      return false;
    }

    // Save file from raw data
    std::optional<UploadedFile> saveFile(const std::string &data, const std::string &originalFilename, const std::string &subdirectory = "")
    {
      try
      {
        std::string ext = getExtension(originalFilename);

        if (!isExtensionAllowed(ext))
        {
          return std::nullopt;
        }

        if (data.size() > maxFileSize)
        {
          return std::nullopt;
        }

        std::string targetDir = uploadDir;
        if (!subdirectory.empty())
        {
          targetDir += "/" + subdirectory;
          ensureDirectoryExists(targetDir);
        }

        std::string storedName = generateUniqueFilename(ext);
        std::string fullPath = targetDir + "/" + storedName;

        std::ofstream file(fullPath, std::ios::binary);
        if (!file.is_open())
        {
          return std::nullopt;
        }

        file.write(data.c_str(), data.size());
        file.close();

        UploadedFile result;
        result.originalName = originalFilename;
        result.storedName = storedName;
        result.path = fullPath;
        result.extension = ext;
        result.mimeType = getMimeType(ext);
        result.size = data.size();

        return result;
      }
      catch (...)
      {
        return std::nullopt;
      }
    }

    // Read file contents
    static std::optional<std::string> readFile(const std::string &filepath)
    {
      try
      {
        std::ifstream file(filepath, std::ios::binary);
        if (!file.is_open())
          return std::nullopt;

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
      }
      catch (...)
      {
        return std::nullopt;
      }
    }

    // Delete file
    static bool deleteFile(const std::string &filepath)
    {
      try
      {
        return fs::remove(filepath);
      }
      catch (...)
      {
        return false;
      }
    }

    // Check if file exists
    static bool exists(const std::string &filepath)
    {
      return fs::exists(filepath);
    }

    // Get file size
    static size_t getFileSize(const std::string &filepath)
    {
      try
      {
        return fs::file_size(filepath);
      }
      catch (...)
      {
        return 0;
      }
    }

    // Get file info
    static std::optional<FileInfo> getFileInfo(const std::string &filepath)
    {
      try
      {
        if (!fs::exists(filepath))
          return std::nullopt;

        FileInfo info;
        info.path = filepath;
        info.name = fs::path(filepath).filename().string();
        info.extension = getExtension(info.name);
        info.isDirectory = fs::is_directory(filepath);
        info.size = info.isDirectory ? 0 : fs::file_size(filepath);

        auto ftime = fs::last_write_time(filepath);
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
        auto time = std::chrono::system_clock::to_time_t(sctp);
        std::ostringstream oss;
        oss << std::put_time(std::gmtime(&time), "%Y-%m-%dT%H:%M:%SZ");
        info.modifiedAt = oss.str();

        return info;
      }
      catch (...)
      {
        return std::nullopt;
      }
    }

    // List files in directory
    static std::vector<FileInfo> listDirectory(const std::string &dirPath)
    {
      std::vector<FileInfo> files;
      try
      {
        for (const auto &entry : fs::directory_iterator(dirPath))
        {
          auto info = getFileInfo(entry.path().string());
          if (info)
          {
            files.push_back(*info);
          }
        }
      }
      catch (...)
      {
      }
      return files;
    }

    // Copy file
    static bool copyFile(const std::string &source, const std::string &destination)
    {
      try
      {
        fs::copy_file(source, destination, fs::copy_options::overwrite_existing);
        return true;
      }
      catch (...)
      {
        return false;
      }
    }

    // Move file
    static bool moveFile(const std::string &source, const std::string &destination)
    {
      try
      {
        fs::rename(source, destination);
        return true;
      }
      catch (...)
      {
        return false;
      }
    }

    // Create download response
    static crow::response downloadResponse(const std::string &filepath, const std::string &downloadName = "")
    {
      auto content = readFile(filepath);
      if (!content)
      {
        return crow::response(404, "File not found");
      }

      std::string filename = downloadName.empty() ? fs::path(filepath).filename().string() : downloadName;
      std::string ext = getExtension(filename);

      crow::response res(200, *content);
      res.add_header("Content-Type", getMimeType(ext));
      res.add_header("Content-Disposition", "attachment; filename=\"" + filename + "\"");
      res.add_header("Content-Length", std::to_string(content->size()));
      return res;
    }

    // Serve file inline (for viewing in browser)
    static crow::response serveFile(const std::string &filepath)
    {
      auto content = readFile(filepath);
      if (!content)
      {
        return crow::response(404, "File not found");
      }

      std::string ext = getExtension(filepath);
      crow::response res(200, *content);
      res.add_header("Content-Type", getMimeType(ext));
      res.add_header("Content-Length", std::to_string(content->size()));
      return res;
    }
  };
}
