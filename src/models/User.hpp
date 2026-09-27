#pragma once

#include <string>
#include <ormpp/dbng.hpp> // Include ormpp header

struct User
{
  int64_t id;
  std::string name;
  std::string email;
  std::string password;
};

// ormpp schema: table "users" with auto-increment primary key "id"
REGISTER_AUTO_KEY(User, id)
REFLECTION_WITH_NAME(User, "users", id, name, email, password)
