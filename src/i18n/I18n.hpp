#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <map>
#include <fstream>
#include <regex>

namespace I18n
{
  using json = nlohmann::json;

  class Translator
  {
  private:
    std::map<std::string, json> translations;
    std::string currentLocale;
    std::string fallbackLocale;

    static Translator &instance()
    {
      static Translator instance;
      return instance;
    }

    Translator() : currentLocale("en"), fallbackLocale("en") {}

  public:
    // Set the current locale
    static void setLocale(const std::string &locale)
    {
      instance().currentLocale = locale;
    }

    // Get the current locale
    static std::string getLocale()
    {
      return instance().currentLocale;
    }

    // Set the fallback locale
    static void setFallbackLocale(const std::string &locale)
    {
      instance().fallbackLocale = locale;
    }

    // Load translations from JSON object
    static void load(const std::string &locale, const json &data)
    {
      instance().translations[locale] = data;
    }

    // Load translations from file
    static bool loadFromFile(const std::string &locale, const std::string &filepath)
    {
      try
      {
        std::ifstream file(filepath);
        if (!file.is_open())
          return false;
        json data;
        file >> data;
        load(locale, data);
        return true;
      }
      catch (...)
      {
        return false;
      }
    }

    // Load all translations from a directory (expects files like en.json, es.json, etc.)
    static void loadFromDirectory(const std::string &dirPath)
    {
      std::vector<std::string> locales = {"en", "es", "fr", "de", "pt", "it", "zh", "ja", "ko", "ru"};
      for (const auto &locale : locales)
      {
        std::string filepath = dirPath + "/" + locale + ".json";
        loadFromFile(locale, filepath);
      }
    }

    // Get translation by key with optional replacements
    static std::string get(const std::string &key, const std::map<std::string, std::string> &replacements = {})
    {
      return instance().translate(key, replacements);
    }

    // Shorthand for get
    static std::string t(const std::string &key, const std::map<std::string, std::string> &replacements = {})
    {
      return get(key, replacements);
    }

    // Get translation for specific locale
    static std::string getForLocale(const std::string &locale, const std::string &key, const std::map<std::string, std::string> &replacements = {})
    {
      std::string savedLocale = instance().currentLocale;
      instance().currentLocale = locale;
      std::string result = get(key, replacements);
      instance().currentLocale = savedLocale;
      return result;
    }

    // Check if translation exists
    static bool has(const std::string &key)
    {
      return instance().hasTranslation(key, instance().currentLocale) ||
             instance().hasTranslation(key, instance().fallbackLocale);
    }

    // Get all available locales
    static std::vector<std::string> getAvailableLocales()
    {
      std::vector<std::string> locales;
      for (const auto &pair : instance().translations)
      {
        locales.push_back(pair.first);
      }
      return locales;
    }

    // Get locale from Accept-Language header
    static std::string parseAcceptLanguage(const std::string &header)
    {
      if (header.empty())
        return instance().fallbackLocale;

      std::regex pattern(R"(([a-zA-Z]{2})(?:-[a-zA-Z]{2})?(?:;q=[\d.]+)?)");
      std::smatch match;
      std::string remaining = header;

      while (std::regex_search(remaining, match, pattern))
      {
        std::string locale = match[1].str();
        std::transform(locale.begin(), locale.end(), locale.begin(), ::tolower);

        if (instance().translations.count(locale))
        {
          return locale;
        }
        remaining = match.suffix();
      }

      return instance().fallbackLocale;
    }

  private:
    std::string translate(const std::string &key, const std::map<std::string, std::string> &replacements)
    {
      std::string value = getNestedValue(key, currentLocale);

      if (value.empty() && currentLocale != fallbackLocale)
      {
        value = getNestedValue(key, fallbackLocale);
      }

      if (value.empty())
      {
        return key;
      }

      // Apply replacements
      for (const auto &pair : replacements)
      {
        std::string placeholder = ":" + pair.first;
        size_t pos = 0;
        while ((pos = value.find(placeholder, pos)) != std::string::npos)
        {
          value.replace(pos, placeholder.length(), pair.second);
          pos += pair.second.length();
        }
      }

      return value;
    }

    std::string getNestedValue(const std::string &key, const std::string &locale)
    {
      if (!translations.count(locale))
        return "";

      json current = translations[locale];
      std::stringstream ss(key);
      std::string part;

      while (std::getline(ss, part, '.'))
      {
        if (!current.is_object() || !current.contains(part))
        {
          return "";
        }
        current = current[part];
      }

      if (current.is_string())
      {
        return current.get<std::string>();
      }

      return "";
    }

    bool hasTranslation(const std::string &key, const std::string &locale)
    {
      return !getNestedValue(key, locale).empty();
    }
  };

  // Convenience function
  inline std::string __(const std::string &key, const std::map<std::string, std::string> &replacements = {})
  {
    return Translator::t(key, replacements);
  }
}
