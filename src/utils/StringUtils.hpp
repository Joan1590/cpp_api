#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <regex>
#include <random>
#include <chrono>
#include <iomanip>

class StringUtils
{
public:
  // Trim whitespace from the beginning of a string
  static std::string ltrim(const std::string &str)
  {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : str.substr(start);
  }

  // Trim whitespace from the end of a string
  static std::string rtrim(const std::string &str)
  {
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
  }

  // Trim whitespace from both ends
  static std::string trim(const std::string &str)
  {
    return rtrim(ltrim(str));
  }

  // Convert string to lowercase
  static std::string toLower(const std::string &str)
  {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
  }

  // Convert string to uppercase
  static std::string toUpper(const std::string &str)
  {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
  }

  // Split string by delimiter
  static std::vector<std::string> split(const std::string &str, char delimiter)
  {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter))
    {
      tokens.push_back(token);
    }
    return tokens;
  }

  // Join vector of strings with delimiter
  static std::string join(const std::vector<std::string> &parts, const std::string &delimiter)
  {
    std::string result;
    for (size_t i = 0; i < parts.size(); ++i)
    {
      result += parts[i];
      if (i < parts.size() - 1)
        result += delimiter;
    }
    return result;
  }

  // Replace all occurrences of a substring
  static std::string replaceAll(const std::string &str, const std::string &from, const std::string &to)
  {
    std::string result = str;
    size_t pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos)
    {
      result.replace(pos, from.length(), to);
      pos += to.length();
    }
    return result;
  }

  // Check if string starts with prefix
  static bool startsWith(const std::string &str, const std::string &prefix)
  {
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
  }

  // Check if string ends with suffix
  static bool endsWith(const std::string &str, const std::string &suffix)
  {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
  }

  // Check if string contains substring
  static bool contains(const std::string &str, const std::string &substr)
  {
    return str.find(substr) != std::string::npos;
  }

  // Generate random string of given length
  static std::string randomString(size_t length)
  {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

    std::string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i)
    {
      result += alphanum[dis(gen)];
    }
    return result;
  }

  // Generate UUID v4
  static std::string generateUUID()
  {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    ss << std::hex;
    for (int i = 0; i < 8; i++)
      ss << dis(gen);
    ss << "-";
    for (int i = 0; i < 4; i++)
      ss << dis(gen);
    ss << "-4";
    for (int i = 0; i < 3; i++)
      ss << dis(gen);
    ss << "-";
    ss << dis2(gen);
    for (int i = 0; i < 3; i++)
      ss << dis(gen);
    ss << "-";
    for (int i = 0; i < 12; i++)
      ss << dis(gen);
    return ss.str();
  }

  // Validate email format
  static bool isValidEmail(const std::string &email)
  {
    const std::regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, pattern);
  }

  // Slugify string (convert to URL-friendly format)
  static std::string slugify(const std::string &str)
  {
    std::string result = toLower(str);
    result = std::regex_replace(result, std::regex("[^a-z0-9\\s-]"), "");
    result = std::regex_replace(result, std::regex("\\s+"), "-");
    result = std::regex_replace(result, std::regex("-+"), "-");
    result = trim(result);
    return result;
  }
};
