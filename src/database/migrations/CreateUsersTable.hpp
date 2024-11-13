#include "../DatabaseManager.hpp"
#include "../../models/User.hpp"
#include <iostream>
#include <ormpp/dbng.hpp>

REGISTER_AUTO_KEY(User, id)
REFLECTION(User, id, name, email, password)

void migrate()
{
  try
  {
    // Instantiate the DatabaseManager
    DatabaseManager dbManager;
    auto &db = dbManager.getDatabase();

    // Create the users table if it doesn't exist
    bool result = db.create_datatable<User>(ormpp_auto_key{"id"});

    if (result)
    {
      std::cout << "Migration completed: Users table created successfully." << std::endl;
    }
    else
    {
      std::cerr << "Migration error: Failed to create users table." << std::endl;
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << "Migration error: " << e.what() << std::endl;
  }
}
