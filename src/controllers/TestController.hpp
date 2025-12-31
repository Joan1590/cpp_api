#pragma once
#include "crow.h"
#include "BaseController.hpp"
#include "../testing/TestFramework.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/DateUtils.hpp"
#include "../validation/Validator.hpp"

namespace Controllers
{
  class TestController : public BaseController
  {
  public:
    // Run all tests and return results
    static crow::response runTests(const crow::request &req)
    {
      try
      {
        Testing::TestRunner runner;

        // String Utils Tests
        Testing::TestSuite stringTests("StringUtils Tests");
        stringTests
            .test("trim removes whitespace", []()
                  {
              Testing::Assertions::assertEqual(std::string("hello"), StringUtils::trim("  hello  ")); })
            .test("toLower converts to lowercase", []()
                  {
              Testing::Assertions::assertEqual(std::string("hello"), StringUtils::toLower("HELLO")); })
            .test("toUpper converts to uppercase", []()
                  {
              Testing::Assertions::assertEqual(std::string("HELLO"), StringUtils::toUpper("hello")); })
            .test("split divides string by delimiter", []()
                  {
              auto parts = StringUtils::split("a,b,c", ',');
              Testing::Assertions::assertEqual(3, (int)parts.size());
              Testing::Assertions::assertEqual(std::string("b"), parts[1]); })
            .test("join combines strings with delimiter", []()
                  {
              std::vector<std::string> parts = {"a", "b", "c"};
              Testing::Assertions::assertEqual(std::string("a-b-c"), StringUtils::join(parts, "-")); })
            .test("isValidEmail validates correct email", []()
                  {
              Testing::Assertions::assertTrue(StringUtils::isValidEmail("test@example.com")); })
            .test("isValidEmail rejects invalid email", []()
                  {
              Testing::Assertions::assertFalse(StringUtils::isValidEmail("invalid-email")); })
            .test("slugify creates URL-friendly string", []()
                  {
              Testing::Assertions::assertEqual(std::string("hello-world"), StringUtils::slugify("Hello World")); })
            .test("startsWith detects prefix", []()
                  {
              Testing::Assertions::assertTrue(StringUtils::startsWith("hello world", "hello")); })
            .test("endsWith detects suffix", []()
                  {
              Testing::Assertions::assertTrue(StringUtils::endsWith("hello world", "world")); })
            .test("contains detects substring", []()
                  {
              Testing::Assertions::assertTrue(StringUtils::contains("hello world", "lo wo")); })
            .test("generateUUID creates valid format", []()
                  {
              auto uuid = StringUtils::generateUUID();
              Testing::Assertions::assertEqual(36, (int)uuid.length()); });

        runner.addSuite(&stringTests);

        // Date Utils Tests
        Testing::TestSuite dateTests("DateUtils Tests");
        dateTests
            .test("now returns ISO format", []()
                  {
              auto now = DateUtils::now();
              Testing::Assertions::assertTrue(now.length() > 0);
              Testing::Assertions::assertContains(now, "T"); })
            .test("timestamp returns positive number", []()
                  {
              Testing::Assertions::assertGreaterThan(DateUtils::timestamp(), (int64_t)0); })
            .test("addDays adds correct days", []()
                  {
              auto now = std::chrono::system_clock::now();
              auto tomorrow = DateUtils::addDays(now, 1);
              auto diff = DateUtils::diffSeconds(now, tomorrow);
              Testing::Assertions::assertEqual((int64_t)86400, diff); })
            .test("isPast returns true for past date", []()
                  {
              auto past = DateUtils::addDays(std::chrono::system_clock::now(), -1);
              Testing::Assertions::assertTrue(DateUtils::isPast(past)); })
            .test("isFuture returns true for future date", []()
                  {
              auto future = DateUtils::addDays(std::chrono::system_clock::now(), 1);
              Testing::Assertions::assertTrue(DateUtils::isFuture(future)); });

        runner.addSuite(&dateTests);

        // Validation Tests
        Testing::TestSuite validationTests("Validation Tests");
        validationTests
            .test("required fails on missing field", []()
                  {
              nlohmann::json data = {};
              auto validator = Validation::make(data).required("name");
              Testing::Assertions::assertTrue(validator.fails()); })
            .test("required passes on present field", []()
                  {
              nlohmann::json data = {{"name", "John"}};
              auto validator = Validation::make(data).required("name");
              Testing::Assertions::assertTrue(validator.passes()); })
            .test("email validates correct format", []()
                  {
              nlohmann::json data = {{"email", "test@example.com"}};
              auto validator = Validation::make(data).email("email");
              Testing::Assertions::assertTrue(validator.passes()); })
            .test("email fails on invalid format", []()
                  {
              nlohmann::json data = {{"email", "invalid"}};
              auto validator = Validation::make(data).email("email");
              Testing::Assertions::assertTrue(validator.fails()); })
            .test("minLength validates string length", []()
                  {
              nlohmann::json data = {{"name", "Jo"}};
              auto validator = Validation::make(data).minLength("name", 3);
              Testing::Assertions::assertTrue(validator.fails()); })
            .test("min validates number value", []()
                  {
              nlohmann::json data = {{"age", 15}};
              auto validator = Validation::make(data).min("age", 18);
              Testing::Assertions::assertTrue(validator.fails()); })
            .test("multiple validations work together", []()
                  {
              nlohmann::json data = {{"name", "John"}, {"email", "john@example.com"}, {"age", 25}};
              auto validator = Validation::make(data)
                .required("name")
                .email("email")
                .min("age", 18);
              Testing::Assertions::assertTrue(validator.passes()); });

        runner.addSuite(&validationTests);

        // Get results as JSON
        json response = runner.toJson();

        // Calculate totals
        int totalPassed = 0;
        int totalFailed = 0;
        for (const auto &suite : response)
        {
          totalPassed += suite["passed"].get<int>();
          totalFailed += suite["failed"].get<int>();
        }

        json finalResponse = {
            {"total_passed", totalPassed},
            {"total_failed", totalFailed},
            {"total_tests", totalPassed + totalFailed},
            {"suites", response}};

        return ok(finalResponse);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Run specific test suite
    static crow::response runSuite(const crow::request &req, const std::string &suiteName)
    {
      try
      {
        if (suiteName == "string")
        {
          Testing::TestSuite suite("StringUtils Tests");
          suite
              .test("trim", []()
                    { Testing::Assertions::assertEqual(std::string("hello"), StringUtils::trim("  hello  ")); })
              .test("toLower", []()
                    { Testing::Assertions::assertEqual(std::string("hello"), StringUtils::toLower("HELLO")); })
              .test("toUpper", []()
                    { Testing::Assertions::assertEqual(std::string("HELLO"), StringUtils::toUpper("hello")); })
              .test("isValidEmail", []()
                    { Testing::Assertions::assertTrue(StringUtils::isValidEmail("test@example.com")); });

          return ok(suite.run().toJson());
        }
        else if (suiteName == "date")
        {
          Testing::TestSuite suite("DateUtils Tests");
          suite
              .test("now", []()
                    { Testing::Assertions::assertTrue(DateUtils::now().length() > 0); })
              .test("timestamp", []()
                    { Testing::Assertions::assertGreaterThan(DateUtils::timestamp(), (int64_t)0); });

          return ok(suite.run().toJson());
        }
        else if (suiteName == "validation")
        {
          Testing::TestSuite suite("Validation Tests");
          suite
              .test("required", []()
                    {
                nlohmann::json data = {{"name", "John"}};
                auto validator = Validation::make(data).required("name");
                Testing::Assertions::assertTrue(validator.passes()); })
              .test("email", []()
                    {
                nlohmann::json data = {{"email", "test@example.com"}};
                auto validator = Validation::make(data).email("email");
                Testing::Assertions::assertTrue(validator.passes()); });

          return ok(suite.run().toJson());
        }

        return not_found("Test suite not found. Available: string, date, validation");
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }
  };
}
