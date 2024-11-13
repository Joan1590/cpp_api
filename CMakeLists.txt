cmake_minimum_required(VERSION 3.15)
project(cpp_api)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)

# MySQL
find_library(MYSQL_LIB mysqlclient PATHS /usr/lib /usr/local/lib)
if(NOT MYSQL_LIB)
    message(FATAL_ERROR "Could not find MySQL client library")
endif()
find_path(MYSQL_INCLUDE_DIR mysql.h
    PATHS /usr/include /usr/local/include /usr/include/mysql
)
if(NOT MYSQL_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find MySQL include directory")
endif()

# Redis configuration
find_path(HIREDIS_INCLUDE_DIR hiredis/hiredis.h
    PATHS /usr/include /usr/local/include
)
find_library(HIREDIS_LIBRARY hiredis
    PATHS /usr/lib /usr/local/lib
)

find_path(REDIS_PLUS_PLUS_INCLUDE_DIR sw/redis++/redis++.h
    PATHS /usr/include /usr/local/include
)
find_library(REDIS_PLUS_PLUS_LIBRARY redis++
    PATHS /usr/lib /usr/local/lib
)

if(NOT REDIS_PLUS_PLUS_INCLUDE_DIR OR NOT REDIS_PLUS_PLUS_LIBRARY)
    message(STATUS "Redis++ include dir: ${REDIS_PLUS_PLUS_INCLUDE_DIR}")
    message(STATUS "Redis++ library: ${REDIS_PLUS_PLUS_LIBRARY}")
    message(FATAL_ERROR "Could not find Redis++ library or headers")
endif()

# --- ormpp Configuration ---
set(ENABLE_MYSQL ON)
add_definitions(-DORMPP_ENABLE_MYSQL)
add_subdirectory(libs/ormpp)

# Add after your existing find_package calls
find_package(OpenSSL REQUIRED)

# Add your executable
add_executable(cpp_api src/main.cpp)

# Include directories
target_include_directories(cpp_api 
    PRIVATE 
    ${Boost_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/libs/ormpp 
    ${MYSQL_INCLUDE_DIR}
    ${HIREDIS_INCLUDE_DIR}
    ${REDIS_PLUS_PLUS_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(cpp_api 
    PRIVATE 
    ${Boost_LIBRARIES}
    nlohmann_json::nlohmann_json
    ${MYSQL_LIB}
    ${HIREDIS_LIBRARY}
    ${REDIS_PLUS_PLUS_LIBRARY}
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Add a custom command to print the include directories (for debugging)
add_custom_target(print_includes ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Include directories:"
    COMMAND ${CMAKE_COMMAND} -E echo ${CMAKE_CXX_INCLUDE_PATH}
)