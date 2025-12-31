#pragma once
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

class DateUtils
{
public:
  using TimePoint = std::chrono::system_clock::time_point;

  // Get current timestamp as string (ISO 8601 format)
  static std::string now()
  {
    return format(std::chrono::system_clock::now());
  }

  // Get current Unix timestamp (seconds since epoch)
  static int64_t timestamp()
  {
    return std::chrono::duration_cast<std::chrono::seconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
  }

  // Get current Unix timestamp in milliseconds
  static int64_t timestampMs()
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
  }

  // Format time_point to ISO 8601 string
  static std::string format(const TimePoint &tp, const std::string &fmt = "%Y-%m-%dT%H:%M:%SZ")
  {
    auto time = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::gmtime(&time);
    std::ostringstream oss;
    oss << std::put_time(&tm, fmt.c_str());
    return oss.str();
  }

  // Parse ISO 8601 string to time_point
  static TimePoint parse(const std::string &dateStr, const std::string &fmt = "%Y-%m-%dT%H:%M:%SZ")
  {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, fmt.c_str());
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
  }

  // Add seconds to a time_point
  static TimePoint addSeconds(const TimePoint &tp, int64_t seconds)
  {
    return tp + std::chrono::seconds(seconds);
  }

  // Add minutes to a time_point
  static TimePoint addMinutes(const TimePoint &tp, int64_t minutes)
  {
    return tp + std::chrono::minutes(minutes);
  }

  // Add hours to a time_point
  static TimePoint addHours(const TimePoint &tp, int64_t hours)
  {
    return tp + std::chrono::hours(hours);
  }

  // Add days to a time_point
  static TimePoint addDays(const TimePoint &tp, int64_t days)
  {
    return tp + std::chrono::hours(days * 24);
  }

  // Get difference between two time_points in seconds
  static int64_t diffSeconds(const TimePoint &start, const TimePoint &end)
  {
    return std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
  }

  // Check if a time_point is in the past
  static bool isPast(const TimePoint &tp)
  {
    return tp < std::chrono::system_clock::now();
  }

  // Check if a time_point is in the future
  static bool isFuture(const TimePoint &tp)
  {
    return tp > std::chrono::system_clock::now();
  }

  // Get start of day
  static TimePoint startOfDay(const TimePoint &tp)
  {
    auto time = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::gmtime(&time);
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
  }

  // Get end of day
  static TimePoint endOfDay(const TimePoint &tp)
  {
    auto time = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::gmtime(&time);
    tm.tm_hour = 23;
    tm.tm_min = 59;
    tm.tm_sec = 59;
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
  }

  // Format for display (human readable)
  static std::string toReadable(const TimePoint &tp)
  {
    return format(tp, "%B %d, %Y at %H:%M");
  }

  // Format date only
  static std::string toDate(const TimePoint &tp)
  {
    return format(tp, "%Y-%m-%d");
  }

  // Format time only
  static std::string toTime(const TimePoint &tp)
  {
    return format(tp, "%H:%M:%S");
  }
};
