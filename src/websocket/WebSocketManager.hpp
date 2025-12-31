#pragma once
#include "crow.h"
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <string>
#include <functional>

namespace WebSocket
{
  using json = nlohmann::json;

  struct Connection
  {
    crow::websocket::connection *conn;
    std::string id;
    std::string userId;
    std::unordered_set<std::string> rooms;
    json metadata;
  };

  class WebSocketManager
  {
  private:
    std::unordered_map<crow::websocket::connection *, Connection> connections;
    std::unordered_map<std::string, std::unordered_set<crow::websocket::connection *>> rooms;
    std::unordered_map<std::string, std::function<void(crow::websocket::connection &, const json &)>> eventHandlers;
    std::mutex mtx;
    int connectionCounter = 0;

    static WebSocketManager &instance()
    {
      static WebSocketManager instance;
      return instance;
    }

    std::string generateConnectionId()
    {
      return "conn_" + std::to_string(++connectionCounter) + "_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    }

  public:
    // Add a new connection
    static void addConnection(crow::websocket::connection &conn)
    {
      std::lock_guard<std::mutex> lock(instance().mtx);

      Connection connection;
      connection.conn = &conn;
      connection.id = instance().generateConnectionId();
      connection.metadata = json::object();

      instance().connections[&conn] = connection;

      // Send welcome message with connection ID
      json welcome = {
          {"type", "connected"},
          {"connectionId", connection.id},
          {"message", "Welcome to WebSocket server"}};
      conn.send_text(welcome.dump());
    }

    // Remove a connection
    static void removeConnection(crow::websocket::connection &conn)
    {
      std::lock_guard<std::mutex> lock(instance().mtx);

      auto it = instance().connections.find(&conn);
      if (it != instance().connections.end())
      {
        // Remove from all rooms
        for (const auto &room : it->second.rooms)
        {
          instance().rooms[room].erase(&conn);
          if (instance().rooms[room].empty())
          {
            instance().rooms.erase(room);
          }
        }
        instance().connections.erase(it);
      }
    }

    // Handle incoming message
    static void handleMessage(crow::websocket::connection &conn, const std::string &data)
    {
      try
      {
        json message = json::parse(data);

        std::string type = message.value("type", "message");
        json payload = message.value("payload", json::object());

        // Handle built-in events
        if (type == "join")
        {
          std::string room = payload.value("room", "");
          if (!room.empty())
          {
            joinRoom(conn, room);
          }
        }
        else if (type == "leave")
        {
          std::string room = payload.value("room", "");
          if (!room.empty())
          {
            leaveRoom(conn, room);
          }
        }
        else if (type == "broadcast")
        {
          broadcast(message.value("data", json::object()), &conn);
        }
        else if (type == "room_message")
        {
          std::string room = payload.value("room", "");
          json msgData = payload.value("data", json::object());
          if (!room.empty())
          {
            sendToRoom(room, msgData, &conn);
          }
        }
        else if (type == "ping")
        {
          json pong = {{"type", "pong"}, {"timestamp", std::chrono::system_clock::now().time_since_epoch().count()}};
          conn.send_text(pong.dump());
        }
        else if (type == "set_user")
        {
          std::string userId = payload.value("userId", "");
          setUserId(conn, userId);
        }
        else
        {
          // Check for custom event handlers
          std::lock_guard<std::mutex> lock(instance().mtx);
          auto handler = instance().eventHandlers.find(type);
          if (handler != instance().eventHandlers.end())
          {
            handler->second(conn, payload);
          }
          else
          {
            // Echo back unknown message types
            json response = {
                {"type", "echo"},
                {"originalType", type},
                {"payload", payload}};
            conn.send_text(response.dump());
          }
        }
      }
      catch (const std::exception &e)
      {
        json error = {
            {"type", "error"},
            {"message", std::string("Failed to parse message: ") + e.what()}};
        conn.send_text(error.dump());
      }
    }

    // Join a room
    static void joinRoom(crow::websocket::connection &conn, const std::string &room)
    {
      std::lock_guard<std::mutex> lock(instance().mtx);

      auto it = instance().connections.find(&conn);
      if (it != instance().connections.end())
      {
        it->second.rooms.insert(room);
        instance().rooms[room].insert(&conn);

        json response = {
            {"type", "room_joined"},
            {"room", room},
            {"message", "Successfully joined room: " + room}};
        conn.send_text(response.dump());

        // Notify others in the room
        json notification = {
            {"type", "user_joined"},
            {"room", room},
            {"connectionId", it->second.id}};

        for (auto *c : instance().rooms[room])
        {
          if (c != &conn)
          {
            c->send_text(notification.dump());
          }
        }
      }
    }

    // Leave a room
    static void leaveRoom(crow::websocket::connection &conn, const std::string &room)
    {
      std::lock_guard<std::mutex> lock(instance().mtx);

      auto it = instance().connections.find(&conn);
      if (it != instance().connections.end())
      {
        it->second.rooms.erase(room);

        auto roomIt = instance().rooms.find(room);
        if (roomIt != instance().rooms.end())
        {
          // Notify others before removing
          json notification = {
              {"type", "user_left"},
              {"room", room},
              {"connectionId", it->second.id}};

          for (auto *c : roomIt->second)
          {
            if (c != &conn)
            {
              c->send_text(notification.dump());
            }
          }

          roomIt->second.erase(&conn);
          if (roomIt->second.empty())
          {
            instance().rooms.erase(roomIt);
          }
        }

        json response = {
            {"type", "room_left"},
            {"room", room},
            {"message", "Successfully left room: " + room}};
        conn.send_text(response.dump());
      }
    }

    // Send message to a specific room
    static void sendToRoom(const std::string &room, const json &data, crow::websocket::connection *exclude = nullptr)
    {
      std::lock_guard<std::mutex> lock(instance().mtx);

      auto it = instance().rooms.find(room);
      if (it != instance().rooms.end())
      {
        json message = {
            {"type", "room_message"},
            {"room", room},
            {"data", data}};
        std::string msg = message.dump();

        for (auto *conn : it->second)
        {
          if (conn != exclude)
          {
            conn->send_text(msg);
          }
        }
      }
    }

    // Broadcast to all connections
    static void broadcast(const json &data, crow::websocket::connection *exclude = nullptr)
    {
      std::lock_guard<std::mutex> lock(instance().mtx);

      json message = {
          {"type", "broadcast"},
          {"data", data}};
      std::string msg = message.dump();

      for (auto &[conn, connection] : instance().connections)
      {
        if (conn != exclude)
        {
          conn->send_text(msg);
        }
      }
    }

    // Send to a specific connection
    static void sendTo(crow::websocket::connection &conn, const json &data)
    {
      json message = {
          {"type", "message"},
          {"data", data}};
      conn.send_text(message.dump());
    }

    // Set user ID for a connection
    static void setUserId(crow::websocket::connection &conn, const std::string &userId)
    {
      std::lock_guard<std::mutex> lock(instance().mtx);

      auto it = instance().connections.find(&conn);
      if (it != instance().connections.end())
      {
        it->second.userId = userId;

        json response = {
            {"type", "user_set"},
            {"userId", userId}};
        conn.send_text(response.dump());
      }
    }

    // Register custom event handler
    static void on(const std::string &event, std::function<void(crow::websocket::connection &, const json &)> handler)
    {
      std::lock_guard<std::mutex> lock(instance().mtx);
      instance().eventHandlers[event] = handler;
    }

    // Get connection count
    static size_t getConnectionCount()
    {
      std::lock_guard<std::mutex> lock(instance().mtx);
      return instance().connections.size();
    }

    // Get room member count
    static size_t getRoomMemberCount(const std::string &room)
    {
      std::lock_guard<std::mutex> lock(instance().mtx);
      auto it = instance().rooms.find(room);
      return it != instance().rooms.end() ? it->second.size() : 0;
    }

    // Get all rooms
    static std::vector<std::string> getRooms()
    {
      std::lock_guard<std::mutex> lock(instance().mtx);
      std::vector<std::string> roomList;
      for (const auto &[room, _] : instance().rooms)
      {
        roomList.push_back(room);
      }
      return roomList;
    }

    // Get connection info
    static json getConnectionInfo(crow::websocket::connection &conn)
    {
      std::lock_guard<std::mutex> lock(instance().mtx);

      auto it = instance().connections.find(&conn);
      if (it != instance().connections.end())
      {
        json info = {
            {"connectionId", it->second.id},
            {"userId", it->second.userId},
            {"rooms", it->second.rooms},
            {"metadata", it->second.metadata}};
        return info;
      }
      return json::object();
    }

    // Get server stats
    static json getStats()
    {
      std::lock_guard<std::mutex> lock(instance().mtx);

      json roomStats = json::object();
      for (const auto &[room, conns] : instance().rooms)
      {
        roomStats[room] = conns.size();
      }

      return {
          {"totalConnections", instance().connections.size()},
          {"totalRooms", instance().rooms.size()},
          {"rooms", roomStats}};
    }
  };
}
