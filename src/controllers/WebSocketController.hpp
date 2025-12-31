#pragma once
#include "crow.h"
#include "BaseController.hpp"
#include "../websocket/WebSocketManager.hpp"

namespace Controllers
{
  class WebSocketController : public BaseController
  {
  public:
    // Get WebSocket server stats via HTTP
    static crow::response getStats(const crow::request &req)
    {
      try
      {
        return ok(WebSocket::WebSocketManager::getStats());
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Get list of active rooms
    static crow::response getRooms(const crow::request &req)
    {
      try
      {
        auto rooms = WebSocket::WebSocketManager::getRooms();
        json response = {
            {"count", rooms.size()},
            {"rooms", rooms}};
        return ok(response);
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Broadcast message to all WebSocket connections via HTTP
    static crow::response broadcast(const crow::request &req)
    {
      try
      {
        auto body = parse_body(req);

        if (!body.contains("message"))
        {
          return bad_request("Message is required");
        }

        WebSocket::WebSocketManager::broadcast(body["message"]);

        return ok({{"message", "Broadcast sent successfully"}});
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }

    // Send message to a specific room via HTTP
    static crow::response sendToRoom(const crow::request &req, const std::string &room)
    {
      try
      {
        auto body = parse_body(req);

        if (!body.contains("message"))
        {
          return bad_request("Message is required");
        }

        WebSocket::WebSocketManager::sendToRoom(room, body["message"]);

        return ok({{"message", "Message sent to room: " + room}});
      }
      catch (const std::exception &e)
      {
        return server_error(e.what());
      }
    }
  };
}
