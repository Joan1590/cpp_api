#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <regex>
#include <functional>
#include <map>

namespace Validation
{
  using json = nlohmann::json;

  struct ValidationError
  {
    std::string field;
    std::string message;
  };

  struct ValidationResult
  {
    bool valid;
    std::vector<ValidationError> errors;

    json toJson() const
    {
      json result;
      result["valid"] = valid;
      result["errors"] = json::array();
      for (const auto &error : errors)
      {
        result["errors"].push_back({{"field", error.field}, {"message", error.message}});
      }
      return result;
    }
  };

  class Validator
  {
  private:
    const json &data;
    std::vector<ValidationError> errors;

  public:
    explicit Validator(const json &jsonData) : data(jsonData) {}

    // Required field validation
    Validator &required(const std::string &field)
    {
      if (!data.contains(field) || data[field].is_null())
      {
        errors.push_back({field, "The " + field + " field is required"});
      }
      return *this;
    }

    // String type validation
    Validator &string(const std::string &field)
    {
      if (data.contains(field) && !data[field].is_null() && !data[field].is_string())
      {
        errors.push_back({field, "The " + field + " field must be a string"});
      }
      return *this;
    }

    // Number type validation
    Validator &number(const std::string &field)
    {
      if (data.contains(field) && !data[field].is_null() && !data[field].is_number())
      {
        errors.push_back({field, "The " + field + " field must be a number"});
      }
      return *this;
    }

    // Integer type validation
    Validator &integer(const std::string &field)
    {
      if (data.contains(field) && !data[field].is_null() && !data[field].is_number_integer())
      {
        errors.push_back({field, "The " + field + " field must be an integer"});
      }
      return *this;
    }

    // Boolean type validation
    Validator &boolean(const std::string &field)
    {
      if (data.contains(field) && !data[field].is_null() && !data[field].is_boolean())
      {
        errors.push_back({field, "The " + field + " field must be a boolean"});
      }
      return *this;
    }

    // Array type validation
    Validator &array(const std::string &field)
    {
      if (data.contains(field) && !data[field].is_null() && !data[field].is_array())
      {
        errors.push_back({field, "The " + field + " field must be an array"});
      }
      return *this;
    }

    // Object type validation
    Validator &object(const std::string &field)
    {
      if (data.contains(field) && !data[field].is_null() && !data[field].is_object())
      {
        errors.push_back({field, "The " + field + " field must be an object"});
      }
      return *this;
    }

    // Email format validation
    Validator &email(const std::string &field)
    {
      if (data.contains(field) && data[field].is_string())
      {
        std::string value = data[field].get<std::string>();
        const std::regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        if (!std::regex_match(value, pattern))
        {
          errors.push_back({field, "The " + field + " field must be a valid email address"});
        }
      }
      return *this;
    }

    // Minimum string length
    Validator &minLength(const std::string &field, size_t min)
    {
      if (data.contains(field) && data[field].is_string())
      {
        std::string value = data[field].get<std::string>();
        if (value.length() < min)
        {
          errors.push_back({field, "The " + field + " field must be at least " + std::to_string(min) + " characters"});
        }
      }
      return *this;
    }

    // Maximum string length
    Validator &maxLength(const std::string &field, size_t max)
    {
      if (data.contains(field) && data[field].is_string())
      {
        std::string value = data[field].get<std::string>();
        if (value.length() > max)
        {
          errors.push_back({field, "The " + field + " field must not exceed " + std::to_string(max) + " characters"});
        }
      }
      return *this;
    }

    // Minimum number value
    Validator &min(const std::string &field, double minVal)
    {
      if (data.contains(field) && data[field].is_number())
      {
        double value = data[field].get<double>();
        if (value < minVal)
        {
          errors.push_back({field, "The " + field + " field must be at least " + std::to_string(minVal)});
        }
      }
      return *this;
    }

    // Maximum number value
    Validator &max(const std::string &field, double maxVal)
    {
      if (data.contains(field) && data[field].is_number())
      {
        double value = data[field].get<double>();
        if (value > maxVal)
        {
          errors.push_back({field, "The " + field + " field must not exceed " + std::to_string(maxVal)});
        }
      }
      return *this;
    }

    // Regex pattern validation
    Validator &pattern(const std::string &field, const std::string &regex, const std::string &message = "")
    {
      if (data.contains(field) && data[field].is_string())
      {
        std::string value = data[field].get<std::string>();
        const std::regex rgx(regex);
        if (!std::regex_match(value, rgx))
        {
          std::string msg = message.empty() ? "The " + field + " field format is invalid" : message;
          errors.push_back({field, msg});
        }
      }
      return *this;
    }

    // In array validation (allowed values)
    template <typename T>
    Validator &in(const std::string &field, const std::vector<T> &allowedValues)
    {
      if (data.contains(field) && !data[field].is_null())
      {
        T value = data[field].get<T>();
        bool found = false;
        for (const auto &allowed : allowedValues)
        {
          if (value == allowed)
          {
            found = true;
            break;
          }
        }
        if (!found)
        {
          errors.push_back({field, "The " + field + " field contains an invalid value"});
        }
      }
      return *this;
    }

    // URL format validation
    Validator &url(const std::string &field)
    {
      if (data.contains(field) && data[field].is_string())
      {
        std::string value = data[field].get<std::string>();
        const std::regex pattern(R"(https?://[^\s/$.?#].[^\s]*)");
        if (!std::regex_match(value, pattern))
        {
          errors.push_back({field, "The " + field + " field must be a valid URL"});
        }
      }
      return *this;
    }

    // Date format validation (YYYY-MM-DD)
    Validator &date(const std::string &field)
    {
      if (data.contains(field) && data[field].is_string())
      {
        std::string value = data[field].get<std::string>();
        const std::regex pattern(R"(\d{4}-\d{2}-\d{2})");
        if (!std::regex_match(value, pattern))
        {
          errors.push_back({field, "The " + field + " field must be a valid date (YYYY-MM-DD)"});
        }
      }
      return *this;
    }

    // Custom validation with callback
    Validator &custom(const std::string &field, std::function<bool(const json &)> validator, const std::string &message)
    {
      if (data.contains(field) && !validator(data[field]))
      {
        errors.push_back({field, message});
      }
      return *this;
    }

    // Confirmed field validation (field must match field_confirmation)
    Validator &confirmed(const std::string &field)
    {
      std::string confirmField = field + "_confirmation";
      if (data.contains(field) && data.contains(confirmField))
      {
        if (data[field] != data[confirmField])
        {
          errors.push_back({field, "The " + field + " confirmation does not match"});
        }
      }
      return *this;
    }

    // Get validation result
    ValidationResult validate()
    {
      return {errors.empty(), errors};
    }

    // Check if validation passed
    bool passes()
    {
      return errors.empty();
    }

    // Check if validation failed
    bool fails()
    {
      return !errors.empty();
    }

    // Get errors as JSON
    json getErrors()
    {
      return validate().toJson();
    }
  };

  // Helper function to create validator
  inline Validator make(const json &data)
  {
    return Validator(data);
  }
}
