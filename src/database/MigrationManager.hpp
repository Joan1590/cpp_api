// src/MigrationManager.hpp
#pragma once

#include <iostream>
#include <vector>

// Include your migration headers
#include "migrations/CreateUsersTable.hpp"

class MigrationManager
{
public:
  void runMigrations()
  {
    std::cout << "Running migrations...\n";

    // Create an array of migration functions
    std::vector<void (*)()> migrations = {
        migrate, // Call CreateUsersTable::migrate()
                 // Add more migration functions here as needed...
    };

    // Execute each migration
    for (auto &migration : migrations)
    {
      migration();
      std::cout << "Migration executed successfully.\n";
    }

    std::cout << "Migrations completed.\n";
  }
};