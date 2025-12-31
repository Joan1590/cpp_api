#pragma once
#include "crow.h"
#include "BaseController.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/DateUtils.hpp"
#include "../utils/JsonUtils.hpp"

namespace Controllers
{
  class UtilsController : public BaseController
  {
  public:
    // String utilities demo
    static crow::response stringUtils(const crow::request &req)
    {
      try
      {
        auto body = parse_body(req);
        std::string text = JsonUtils::get<std::string>(body, "text", "");

        if (text.empty())
        {
          return bad_request("The 'text' field is required");
        }

        json response = {
            {"original", text},
            {"trimmed", StringUtils::trim(text)},
            {"lowercase", StringUtils::toLower(text)},
            {"uppercase", StringUtils::toUpper(text)},
            {"slugified", StringUtils::slugify(text)},
            {"is_valid_email", StringUtils::isValidEmail(text)},
            {"starts_with_hello", StringUtils::startsWith(text, "hello")},
            {"contains_test", StringUtils::contains(text, "test")}};

        return ok(response);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Date utilities demo
    static crow::response dateUtils(const crow::request &req)
    {
      try
      {
        auto now = std::chrono::system_clock::now();

        json response = {
            {"now_iso", DateUtils::now()},
            {"timestamp", DateUtils::timestamp()},
            {"timestamp_ms", DateUtils::timestampMs()},
            {"readable", DateUtils::toReadable(now)},
            {"date_only", DateUtils::toDate(now)},
            {"time_only", DateUtils::toTime(now)},
            {"tomorrow", DateUtils::format(DateUtils::addDays(now, 1))},
            {"next_hour", DateUtils::format(DateUtils::addHours(now, 1))}};

        return ok(response);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Generate random values
    static crow::response generateRandom(const crow::request &req)
    {
      try
      {
        json response = {
            {"uuid", StringUtils::generateUUID()},
            {"random_string_8", StringUtils::randomString(8)},
            {"random_string_16", StringUtils::randomString(16)},
            {"random_string_32", StringUtils::randomString(32)}};

        return ok(response);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // JSON utilities demo
    static crow::response jsonUtils(const crow::request &req)
    {
      try
      {
        auto body = parse_body(req);

        // Demonstrate various JSON utilities
        std::vector<std::string> requiredKeys = {"name", "email"};
        auto missingKeys = JsonUtils::getMissingKeys(body, requiredKeys);

        json base = {{"default_value", true}, {"version", "1.0"}};
        json merged = JsonUtils::merge(base, body);

        json response = {
            {"original", body},
            {"has_required_keys", JsonUtils::hasKeys(body, requiredKeys)},
            {"missing_keys", missingKeys},
            {"merged_with_defaults", merged},
            {"pretty_printed", JsonUtils::prettyPrint(body, 2)}};

        return ok(response);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }
  };
}
