# C++ API Framework (CrowCpp)

This project provides a lightweight and efficient C++ API framework, inspired by Laravel's elegant design, built upon the CrowCpp microframework. It focuses solely on API functionalities, making it ideal for backend development and microservices.

## Features

- **Fast and efficient:** Leverages CrowCpp's performance for handling HTTP requests.
- **Database connectivity:** Supports seamless integration with MySQL and PostgreSQL.
- **Middleware:** Provides middleware support for request processing and manipulation.
- **JWT authentication:** Implements JWT (JSON Web Token) for secure API access.
- **Redis integration:** Utilizes Redis for caching and session management.
- **Easy routing:** Offers a simple and intuitive routing system.
- **Dockerized environment:** Includes Dockerfile and docker-compose for easy setup and deployment.

## Folder Structure

Markdown

# C++ API Framework (CrowCpp)

This project provides a lightweight and efficient C++ API framework, inspired by Laravel's elegant design, built upon the CrowCpp microframework. It focuses solely on API functionalities, making it ideal for backend development and microservices.

## Features

- **Fast and efficient:** Leverages CrowCpp's performance for handling HTTP requests.
- **Database connectivity:** Supports seamless integration with MySQL and PostgreSQL.
- **Middleware:** Provides middleware support for request processing and manipulation.
- **JWT authentication:** Implements JWT (JSON Web Token) for secure API access.
- **Redis integration:** Utilizes Redis for caching and session management.
- **Easy routing:** Offers a simple and intuitive routing system.
- **Dockerized environment:** Includes Dockerfile and docker-compose for easy setup and deployment.

## Folder Structure

```text
├── src
│ ├── libs
│ ├── scripts
│ │ └── watch.sh
│ ├── config
│ │ └── hconfig.hpp
│ ├── controllers
│ │ ├── h AuthController.hpp
│ │ ├── h BaseController.hpp
│ │ ├── HelloController.hpp
│ │ └── h™ UserController.hpp
│ ├── database
│ │ └── migrations
│ │ ├── CreateUsersTable...
│ │ ├── DatabaseManager....
│ │ └── MigrationManager....
│ ├── middlewares
│ │ ├── JWTMiddleware.hpp
│ │ └── NoMiddleware.hpp
│ ├── models
│ │ └── hUser.hpp
│ ├── redis
│ │ └── RedisManager.hpp
│ ├── routes
│ │ ├── AuthRouter.hpp
│ │ ├── HelloRouter.hpp
│ │ ├── RouteManager.hpp
│ │ ├── h Router.hpp
│ │ └── UserRouter.hpp
│ └── utils
│ └── h HashUtils.hpp
├── main.cpp
├── .dockerignore
├── .gitignore
├── .gitmodules
├── CMakeLists.txt
├── docker-compose.yml
└── Dockerfile
```

## Getting Started

1. **Clone the repository:**

   ```bash
   git clone https://github.com/joan1590/crowcpp-api.git
   ```

2. **Build the project:**

   ```bash
   docker-compose up -d --build
   ```

3. **Run the API:**

The API will be accessible at http://localhost:3000

## Pending Features

[x] Fast and efficient API using CrowCpp.
[x] Database connectivity (MySQL, PostgreSQL).
[ ] Middleware:
[ ] Apply middleware to specific routes.
[x] JWT authentication.
[x] Redis integration.
[ ] Background tasks and events.
[ ] WebSocket support.
[ ] Utility functions in utils.
[ ] Request validation.
[ ] RabbitMQ integration.
[ ] Data export (PDF, CSV).
[ ] Internationalization (i18n).
[ ] File handling (upload, save, delete).
[ ] Testing framework.
[x] Dockerized environment.
[ ] Command to generate files.
