#pragma once

#include "../DatabaseManager.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <tuple>

// Older versions created the users table as `User` (the struct name) because the model
// had no explicit table name. Rename it to `users` so existing data is kept.
inline void renameUserTable()
{
  try
  {
    DatabaseManager dbManager;
    auto &db = dbManager.getDatabase();

    auto tables = db.query_s<std::tuple<std::string>>(
        "SELECT table_name FROM information_schema.tables "
        "WHERE table_schema = DATABASE() AND LOWER(table_name) IN ('user', 'users')");

    if (tables.empty() && !db.get_last_error().empty())
    {
      std::cerr << "Migration error: failed to inspect tables: " << db.get_last_error() << std::endl;
      return;
    }

    std::string legacyName;
    bool hasUsersTable = false;
    for (const auto &row : tables)
    {
      std::string name = std::get<0>(row);
      std::string lower = name;
      std::transform(lower.begin(), lower.end(), lower.begin(),
                     [](unsigned char c) { return std::tolower(c); });

      if (lower == "users")
      {
        hasUsersTable = true;
      }
      else if (lower == "user")
      {
        // "User", or "user" on servers with lower_case_table_names != 0
        legacyName = name;
      }
    }

    if (legacyName.empty())
    {
      std::cout << "Migration skipped: no legacy `User` table found." << std::endl;
      return;
    }

    if (hasUsersTable)
    {
      std::cerr << "Migration warning: both `" << legacyName
                << "` and `users` exist; merge them manually." << std::endl;
      return;
    }

    if (!db.execute("RENAME TABLE `" + legacyName + "` TO `users`"))
    {
      std::cerr << "Migration error: failed to rename `" << legacyName
                << "` to `users`: " << db.get_last_error() << std::endl;
      return;
    }

    std::cout << "Migration completed: `" << legacyName << "` renamed to `users`." << std::endl;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Migration error: " << e.what() << std::endl;
  }
}
