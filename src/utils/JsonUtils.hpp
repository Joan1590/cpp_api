#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <optional>
#include <fstream>

class JsonUtils
{
public:
  using json = nlohmann::json;

  // Safe get value with default
  template <typename T>
  static T get(const json &j, const std::string &key, const T &defaultValue)
  {
    try
    {
      if (j.contains(key) && !j[key].is_null())
      {
        return j[key].get<T>();
      }
    }
    catch (...)
    {
    }
    return defaultValue;
  }

  // Safe get optional value
  template <typename T>
  static std::optional<T> getOptional(const json &j, const std::string &key)
  {
    try
    {
      if (j.contains(key) && !j[key].is_null())
      {
        return j[key].get<T>();
      }
    }
    catch (...)
    {
    }
    return std::nullopt;
  }

  // Check if JSON has all required keys
  static bool hasKeys(const json &j, const std::vector<std::string> &keys)
  {
    for (const auto &key : keys)
    {
      if (!j.contains(key))
        return false;
    }
    return true;
  }

  // Get list of missing keys
  static std::vector<std::string> getMissingKeys(const json &j, const std::vector<std::string> &keys)
  {
    std::vector<std::string> missing;
    for (const auto &key : keys)
    {
      if (!j.contains(key))
        missing.push_back(key);
    }
    return missing;
  }

  // Merge two JSON objects (second overwrites first)
  static json merge(const json &base, const json &override)
  {
    json result = base;
    for (auto it = override.begin(); it != override.end(); ++it)
    {
      result[it.key()] = it.value();
    }
    return result;
  }

  // Deep merge (recursive)
  static json deepMerge(const json &base, const json &override)
  {
    json result = base;
    for (auto it = override.begin(); it != override.end(); ++it)
    {
      if (result.contains(it.key()) && result[it.key()].is_object() && it.value().is_object())
      {
        result[it.key()] = deepMerge(result[it.key()], it.value());
      }
      else
      {
        result[it.key()] = it.value();
      }
    }
    return result;
  }

  // Pretty print JSON
  static std::string prettyPrint(const json &j, int indent = 2)
  {
    return j.dump(indent);
  }

  // Parse JSON safely
  static std::optional<json> parse(const std::string &str)
  {
    try
    {
      return json::parse(str);
    }
    catch (...)
    {
      return std::nullopt;
    }
  }

  // Load JSON from file
  static std::optional<json> loadFromFile(const std::string &filepath)
  {
    try
    {
      std::ifstream file(filepath);
      if (!file.is_open())
        return std::nullopt;
      json j;
      file >> j;
      return j;
    }
    catch (...)
    {
      return std::nullopt;
    }
  }

  // Save JSON to file
  static bool saveToFile(const json &j, const std::string &filepath, int indent = 2)
  {
    try
    {
      std::ofstream file(filepath);
      if (!file.is_open())
        return false;
      file << j.dump(indent);
      return true;
    }
    catch (...)
    {
      return false;
    }
  }

  // Filter JSON object by keys
  static json filterKeys(const json &j, const std::vector<std::string> &keys)
  {
    json result = json::object();
    for (const auto &key : keys)
    {
      if (j.contains(key))
      {
        result[key] = j[key];
      }
    }
    return result;
  }

  // Exclude keys from JSON object
  static json excludeKeys(const json &j, const std::vector<std::string> &keys)
  {
    json result = j;
    for (const auto &key : keys)
    {
      result.erase(key);
    }
    return result;
  }
};
