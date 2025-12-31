#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <map>
#include <nlohmann/json.hpp>

namespace Testing
{
  using json = nlohmann::json;

  struct TestResult
  {
    std::string name;
    bool passed;
    std::string message;
    double durationMs;
  };

  struct TestSuiteResult
  {
    std::string name;
    std::vector<TestResult> tests;
    int passed;
    int failed;
    double totalDurationMs;

    json toJson() const
    {
      json result;
      result["suite"] = name;
      result["passed"] = passed;
      result["failed"] = failed;
      result["total"] = passed + failed;
      result["duration_ms"] = totalDurationMs;
      result["tests"] = json::array();
      for (const auto &test : tests)
      {
        result["tests"].push_back({{"name", test.name},
                                   {"passed", test.passed},
                                   {"message", test.message},
                                   {"duration_ms", test.durationMs}});
      }
      return result;
    }
  };

  class Assertions
  {
  public:
    static void assertTrue(bool condition, const std::string &message = "Expected true")
    {
      if (!condition)
        throw std::runtime_error(message);
    }

    static void assertFalse(bool condition, const std::string &message = "Expected false")
    {
      if (condition)
        throw std::runtime_error(message);
    }

    template <typename T>
    static void assertEqual(const T &expected, const T &actual, const std::string &message = "")
    {
      if (expected != actual)
      {
        std::ostringstream oss;
        oss << (message.empty() ? "Equality assertion failed" : message);
        oss << " - Expected: " << expected << ", Actual: " << actual;
        throw std::runtime_error(oss.str());
      }
    }

    template <typename T>
    static void assertNotEqual(const T &expected, const T &actual, const std::string &message = "")
    {
      if (expected == actual)
      {
        std::ostringstream oss;
        oss << (message.empty() ? "Inequality assertion failed" : message);
        oss << " - Values should not be equal: " << expected;
        throw std::runtime_error(oss.str());
      }
    }

    static void assertNull(const void *ptr, const std::string &message = "Expected null")
    {
      if (ptr != nullptr)
        throw std::runtime_error(message);
    }

    static void assertNotNull(const void *ptr, const std::string &message = "Expected not null")
    {
      if (ptr == nullptr)
        throw std::runtime_error(message);
    }

    template <typename T>
    static void assertGreaterThan(const T &value, const T &threshold, const std::string &message = "")
    {
      if (!(value > threshold))
      {
        std::ostringstream oss;
        oss << (message.empty() ? "Greater than assertion failed" : message);
        oss << " - " << value << " is not greater than " << threshold;
        throw std::runtime_error(oss.str());
      }
    }

    template <typename T>
    static void assertLessThan(const T &value, const T &threshold, const std::string &message = "")
    {
      if (!(value < threshold))
      {
        std::ostringstream oss;
        oss << (message.empty() ? "Less than assertion failed" : message);
        oss << " - " << value << " is not less than " << threshold;
        throw std::runtime_error(oss.str());
      }
    }

    static void assertContains(const std::string &haystack, const std::string &needle, const std::string &message = "")
    {
      if (haystack.find(needle) == std::string::npos)
      {
        std::ostringstream oss;
        oss << (message.empty() ? "String does not contain expected substring" : message);
        throw std::runtime_error(oss.str());
      }
    }

    template <typename ExceptionType>
    static void assertThrows(std::function<void()> func, const std::string &message = "Expected exception not thrown")
    {
      try
      {
        func();
        throw std::runtime_error(message);
      }
      catch (const ExceptionType &)
      {
        // Expected exception was thrown
      }
      catch (const std::runtime_error &e)
      {
        if (std::string(e.what()) == message)
          throw;
      }
    }

    static void assertJsonEqual(const json &expected, const json &actual, const std::string &message = "")
    {
      if (expected != actual)
      {
        std::ostringstream oss;
        oss << (message.empty() ? "JSON equality assertion failed" : message);
        throw std::runtime_error(oss.str());
      }
    }

    static void assertJsonHasKey(const json &j, const std::string &key, const std::string &message = "")
    {
      if (!j.contains(key))
      {
        std::ostringstream oss;
        oss << (message.empty() ? "JSON missing key: " + key : message);
        throw std::runtime_error(oss.str());
      }
    }

    static void fail(const std::string &message = "Test failed")
    {
      throw std::runtime_error(message);
    }
  };

  class TestSuite
  {
  private:
    std::string name;
    std::vector<std::pair<std::string, std::function<void()>>> tests;
    std::function<void()> beforeEach;
    std::function<void()> afterEach;
    std::function<void()> beforeAll;
    std::function<void()> afterAll;

  public:
    explicit TestSuite(const std::string &suiteName) : name(suiteName) {}

    TestSuite &test(const std::string &testName, std::function<void()> testFunc)
    {
      tests.push_back({testName, testFunc});
      return *this;
    }

    TestSuite &setBeforeEach(std::function<void()> func)
    {
      beforeEach = func;
      return *this;
    }

    TestSuite &setAfterEach(std::function<void()> func)
    {
      afterEach = func;
      return *this;
    }

    TestSuite &setBeforeAll(std::function<void()> func)
    {
      beforeAll = func;
      return *this;
    }

    TestSuite &setAfterAll(std::function<void()> func)
    {
      afterAll = func;
      return *this;
    }

    TestSuiteResult run()
    {
      TestSuiteResult result;
      result.name = name;
      result.passed = 0;
      result.failed = 0;

      auto suiteStart = std::chrono::high_resolution_clock::now();

      if (beforeAll)
      {
        try
        {
          beforeAll();
        }
        catch (const std::exception &e)
        {
          std::cerr << "beforeAll failed: " << e.what() << std::endl;
        }
      }

      for (const auto &[testName, testFunc] : tests)
      {
        TestResult testResult;
        testResult.name = testName;

        auto start = std::chrono::high_resolution_clock::now();

        try
        {
          if (beforeEach)
            beforeEach();

          testFunc();

          if (afterEach)
            afterEach();

          testResult.passed = true;
          testResult.message = "OK";
          result.passed++;
        }
        catch (const std::exception &e)
        {
          testResult.passed = false;
          testResult.message = e.what();
          result.failed++;
        }

        auto end = std::chrono::high_resolution_clock::now();
        testResult.durationMs = std::chrono::duration<double, std::milli>(end - start).count();

        result.tests.push_back(testResult);
      }

      if (afterAll)
      {
        try
        {
          afterAll();
        }
        catch (const std::exception &e)
        {
          std::cerr << "afterAll failed: " << e.what() << std::endl;
        }
      }

      auto suiteEnd = std::chrono::high_resolution_clock::now();
      result.totalDurationMs = std::chrono::duration<double, std::milli>(suiteEnd - suiteStart).count();

      return result;
    }

    void runAndPrint()
    {
      auto result = run();
      printResult(result);
    }

    static void printResult(const TestSuiteResult &result)
    {
      std::cout << "\n====================================\n";
      std::cout << "Test Suite: " << result.name << "\n";
      std::cout << "====================================\n\n";

      for (const auto &test : result.tests)
      {
        std::cout << (test.passed ? "✓" : "✗") << " " << test.name;
        std::cout << " (" << std::fixed << std::setprecision(2) << test.durationMs << "ms)";
        if (!test.passed)
        {
          std::cout << "\n  Error: " << test.message;
        }
        std::cout << "\n";
      }

      std::cout << "\n------------------------------------\n";
      std::cout << "Passed: " << result.passed << ", Failed: " << result.failed;
      std::cout << ", Total: " << (result.passed + result.failed);
      std::cout << ", Duration: " << std::fixed << std::setprecision(2) << result.totalDurationMs << "ms\n";
      std::cout << "====================================\n\n";
    }
  };

  class TestRunner
  {
  private:
    std::vector<TestSuite *> suites;

  public:
    void addSuite(TestSuite *suite)
    {
      suites.push_back(suite);
    }

    std::vector<TestSuiteResult> runAll()
    {
      std::vector<TestSuiteResult> results;
      for (auto *suite : suites)
      {
        results.push_back(suite->run());
      }
      return results;
    }

    void runAllAndPrint()
    {
      int totalPassed = 0;
      int totalFailed = 0;

      for (auto *suite : suites)
      {
        auto result = suite->run();
        TestSuite::printResult(result);
        totalPassed += result.passed;
        totalFailed += result.failed;
      }

      std::cout << "\n=====================================\n";
      std::cout << "TOTAL: Passed: " << totalPassed << ", Failed: " << totalFailed << "\n";
      std::cout << "=====================================\n";
    }

    json toJson()
    {
      auto results = runAll();
      json output = json::array();
      for (const auto &result : results)
      {
        output.push_back(result.toJson());
      }
      return output;
    }
  };
}
