#pragma once

#include <string>
#include <ormpp/dbng.hpp> // Include ormpp header

struct User
{
  int64_t id;
  std::string name;
  std::string email;
  std::string password;

  // Define the schema for ormpp
  static constexpr auto table_name = "users";
};