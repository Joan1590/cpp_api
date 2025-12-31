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
│   └── watch.sh
├── src
│   ├── config
│   │   └── config.hpp
│   ├── controllers
│   │   ├── AuthController.hpp
│   │   ├── BaseController.hpp
│   │   ├── ExportController.hpp
│   │   ├── FileController.hpp
│   │   ├── HelloController.hpp
│   │   ├── I18nController.hpp
│   │   ├── TestController.hpp
│   │   ├── UserController.hpp
│   │   ├── UtilsController.hpp
│   │   └── ValidationController.hpp
│   ├── database
│   │   ├── DatabaseManager.hpp
│   │   ├── MigrationManager.hpp
│   │   └── migrations/
│   ├── events
│   │   ├── EventManager.hpp
│   │   └── logEvent.hpp
│   ├── export
│   │   └── DataExporter.hpp
│   ├── files
│   │   └── FileHandler.hpp
│   ├── i18n
│   │   └── I18n.hpp
│   ├── middlewares
│   │   ├── JWTMiddleware.hpp
│   │   └── NoMiddleware.hpp
│   ├── models
│   │   └── User.hpp
│   ├── redis
│   │   └── RedisManager.hpp
│   ├── routes
│   │   ├── AuthRouter.hpp
│   │   ├── ExportRouter.hpp
│   │   ├── FileRouter.hpp
│   │   ├── HelloRouter.hpp
│   │   ├── I18nRouter.hpp
│   │   ├── RouteManager.hpp
│   │   ├── Router.hpp
│   │   ├── TestRouter.hpp
│   │   ├── UserRouter.hpp
│   │   ├── UtilsRouter.hpp
│   │   └── ValidationRouter.hpp
│   ├── testing
│   │   └── TestFramework.hpp
│   ├── utils
│   │   ├── DateUtils.hpp
│   │   ├── HashUtils.hpp
│   │   ├── JsonUtils.hpp
│   │   └── StringUtils.hpp
│   ├── validation
│   │   └── Validator.hpp
│   └── main.cpp
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

## Features Status

```table
| Feature                 | Status      | Notes                                      |
| ----------------------- | ----------- | -----------------------------------------  |
| Fast and efficient API  | Complete    | Uses CrowCpp.                              |
| Database connectivity   | Complete    | Supports MySQL, PostgreSQL.                |
| Middleware              | Complete    | Apply middleware to specific routes.       |
| JWT authentication      | Complete    |                                            |
| Redis integration       | Complete    |                                            |
| Background tasks/events | Complete    |                                            |
| WebSocket support       | Complete    | Real-time bidirectional communication      |
| Utility functions       | Complete    | StringUtils, DateUtils, JsonUtils          |
| Request validation      | Complete    | Fluent validation API                      |
| Data export (CSV, XML)  | Complete    | CSV, XML, HTML, Text table formats         |
| Internationalization    | Complete    | Multi-language support (en, es, fr, pt)    |
| File handling           | Complete    | Upload, download, serve files              |
| Testing framework       | Complete    | Built-in test runner with assertions       |
| Dockerized environment  | Complete    |                                            |
```

## API Endpoints

### Utility Functions
- `POST /api/utils/string` - String manipulation utilities
- `GET /api/utils/date` - Date/time utilities
- `GET /api/utils/random` - Generate random values (UUID, strings)
- `POST /api/utils/json` - JSON utilities

### Request Validation
- `POST /api/validate/user` - Validate user registration data
- `POST /api/validate/product` - Validate product data
- `POST /api/validate/custom` - Custom validation example

### Data Export
- `GET /api/export/users/csv` - Export users to CSV
- `GET /api/export/users/xml` - Export users to XML
- `POST /api/export/csv` - Export custom data to CSV
- `POST /api/export/html` - Export custom data to HTML table
- `POST /api/export/text` - Export custom data to text table

### Internationalization (i18n)
- `POST /api/i18n/translate` - Translate a key
- `GET /api/i18n/locales` - Get available locales
- `GET /api/i18n/demo` - Demo translations in all languages
- `POST /api/i18n/locale` - Set current locale

### File Handling
- `POST /api/files/upload` - Upload a file
- `GET /api/files` - List all uploaded files
- `GET /api/files/<filename>/info` - Get file info
- `GET /api/files/<filename>/download` - Download a file
- `GET /api/files/<filename>/view` - View/serve file inline
- `DELETE /api/files/<filename>` - Delete a file (requires JWT)

### Testing
- `GET /api/tests/run` - Run all tests
- `GET /api/tests/run/<suite>` - Run specific test suite (string, date, validation)

### WebSocket
- `WS /ws` - WebSocket endpoint for real-time communication
- `GET /api/ws/stats` - Get WebSocket server statistics
- `GET /api/ws/rooms` - Get list of active rooms
- `POST /api/ws/broadcast` - Broadcast message to all connections (requires JWT)
- `POST /api/ws/rooms/<room>/send` - Send message to specific room (requires JWT)

## Example Requests (curl)

### Utility Functions
```bash
# String utilities
curl -X POST http://localhost:3000/api/utils/string \
  -H "Content-Type: application/json" \
  -d '{"text": "  Hello World  "}'

# Date utilities
curl http://localhost:3000/api/utils/date

# Generate random values
curl http://localhost:3000/api/utils/random
```

### Request Validation
```bash
# Validate user registration
curl -X POST http://localhost:3000/api/validate/user \
  -H "Content-Type: application/json" \
  -d '{"name": "John", "email": "john@example.com", "password": "password123", "password_confirmation": "password123", "age": 25}'

# Validate product
curl -X POST http://localhost:3000/api/validate/product \
  -H "Content-Type: application/json" \
  -d '{"name": "Laptop", "price": 999.99, "category": "electronics"}'
```

### Data Export
```bash
# Export users to CSV
curl http://localhost:3000/api/export/users/csv

# Export custom data to CSV
curl -X POST http://localhost:3000/api/export/csv \
  -H "Content-Type: application/json" \
  -d '{"data": [{"id": 1, "name": "John"}, {"id": 2, "name": "Jane"}], "filename": "people.csv"}'
```

### Internationalization
```bash
# Get available locales
curl http://localhost:3000/api/i18n/locales

# Demo translations
curl http://localhost:3000/api/i18n/demo

# Translate a key
curl -X POST http://localhost:3000/api/i18n/translate \
  -H "Content-Type: application/json" \
  -H "Accept-Language: es" \
  -d '{"key": "messages.welcome"}'
```

### File Handling
```bash
# Upload a file
curl -X POST http://localhost:3000/api/files/upload \
  -H "X-Filename: test.txt" \
  -d "Hello, this is a test file content"

# List files
curl http://localhost:3000/api/files

# Download a file
curl http://localhost:3000/api/files/test.txt/download -o downloaded.txt
```

### Testing Framework
```bash
# Run all tests
curl http://localhost:3000/api/tests/run

# Run specific test suite
curl http://localhost:3000/api/tests/run/string
curl http://localhost:3000/api/tests/run/validation
```

### WebSocket
```bash
# Get WebSocket stats
curl http://localhost:3000/api/ws/stats

# Get active rooms
curl http://localhost:3000/api/ws/rooms

# Connect using websocat (install: cargo install websocat)
websocat ws://localhost:3000/ws

# Or using wscat (install: npm install -g wscat)
wscat -c ws://localhost:3000/ws
```

## WebSocket Message Protocol

The WebSocket server uses JSON messages with the following format:

```json
{
  "type": "event_type",
  "payload": { ... }
}
```

### Built-in Events

**Client → Server:**

| Event | Payload | Description |
|-------|---------|-------------|
| `ping` | - | Check connection, server responds with `pong` |
| `join` | `{"room": "room_name"}` | Join a chat room |
| `leave` | `{"room": "room_name"}` | Leave a chat room |
| `broadcast` | `{"data": {...}}` | Broadcast to all connections |
| `room_message` | `{"room": "room_name", "data": {...}}` | Send message to a room |
| `set_user` | `{"userId": "user123"}` | Set user ID for connection |
| `chat` | `{"room": "general", "sender": "John", "message": "Hello!"}` | Send chat message |
| `typing` | `{"room": "general", "user": "John", "isTyping": true}` | Typing indicator |

**Server → Client:**

| Event | Description |
|-------|-------------|
| `connected` | Connection established with `connectionId` |
| `pong` | Response to ping |
| `room_joined` | Successfully joined a room |
| `room_left` | Successfully left a room |
| `user_joined` | Another user joined the room |
| `user_left` | Another user left the room |
| `room_message` | Message received in a room |
| `broadcast` | Broadcast message received |
| `chat` | Chat message received |
| `typing` | Typing indicator update |
| `error` | Error message |

### WebSocket Example Session

```bash
# 1. Connect to WebSocket
wscat -c ws://localhost:3000/ws

# Server responds:
# {"type":"connected","connectionId":"conn_1_...","message":"Welcome to WebSocket server"}

# 2. Join a room
> {"type":"join","payload":{"room":"general"}}
# Response: {"type":"room_joined","room":"general","message":"Successfully joined room: general"}

# 3. Send a chat message
> {"type":"chat","payload":{"room":"general","sender":"John","message":"Hello everyone!"}}

# 4. Send typing indicator
> {"type":"typing","payload":{"room":"general","user":"John","isTyping":true}}

# 5. Ping the server
> {"type":"ping","payload":{}}
# Response: {"type":"pong","timestamp":...}

# 6. Leave a room
> {"type":"leave","payload":{"room":"general"}}
```
