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
├── libs
├── scripts
│ └── watch.sh
├── src
│ ├── config
│ │ └── hconfig.hpp
│ ├── controllers
│ │ ├── AuthController.hpp
│ │ ├── BaseController.hpp
│ │ ├── HelloController.hpp
│ │ └── UserController.hpp
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
│ │ ├── Router.hpp
│ │ └── UserRouter.hpp
│ └── utils
│ └── HashUtils.hpp
├── main.cpp
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

```table
| Feature                 | Status      | Notes                                      |
| ----------------------- | ----------- | -----------------------------------------  |
| Fast and efficient API  | Complete    | Uses CrowCpp.                              |
| Database connectivity   | Complete    | Supports MySQL, PostgreSQL.                |
| Middleware              | Complete    | Apply middleware to specific routes.       |
| JWT authentication      | Complete    |                                            |
| Redis integration       | Complete    |                                            |
| Background tasks/events | Complete    |                                            |
| WebSocket support       | In Progress |                                            |
| Utility functions       | Pending     | Add common utilities to the `utils` folder |
| Request validation      | Pending     |                                            |
| Data export (PDF, CSV)  | Pending     |                                            |
| Internationalization    | Pending     |                                            |
| File handling           | Pending     |                                            |
| Testing framework       | Pending     |                                            |
| Dockerized environment  | Complete    |                                            |
```
