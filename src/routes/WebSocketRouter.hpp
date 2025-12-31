#pragma once
#include "Router.hpp"
#include "../controllers/WebSocketController.hpp"
#include "../websocket/WebSocketManager.hpp"

namespace Routes
{
  class WebSocketRouter : public Router
  {
  public:
    void register_routes(crow::App<Middlewares::JWTMiddleware> &app) override
    {
      // Register custom WebSocket event handlers
      registerCustomHandlers();

      // WebSocket endpoint
      CROW_WEBSOCKET_ROUTE(app, "/ws")
          .onopen([](crow::websocket::connection &conn)
                  {
            CROW_LOG_INFO << "WebSocket connection opened";
            WebSocket::WebSocketManager::addConnection(conn); })
          .onclose([](crow::websocket::connection &conn, const std::string &reason, uint16_t code)
                   {
            CROW_LOG_INFO << "WebSocket connection closed: " << reason << " (code: " << code << ")";
            WebSocket::WebSocketManager::removeConnection(conn); })
          .onmessage([](crow::websocket::connection &conn, const std::string &data, bool is_binary)
                     {
            if (!is_binary) {
              WebSocket::WebSocketManager::handleMessage(conn, data);
            } });

      // HTTP endpoints for WebSocket management
      CROW_ROUTE(app, "/api/ws/stats")
          .methods("GET"_method)([](const crow::request &req)
                                 { return Controllers::WebSocketController::getStats(req); });

      CROW_ROUTE(app, "/api/ws/rooms")
          .methods("GET"_method)([](const crow::request &req)
                                 { return Controllers::WebSocketController::getRooms(req); });

      CROW_ROUTE(app, "/api/ws/broadcast")
          .CROW_MIDDLEWARES(app, Middlewares::JWTMiddleware)
          .methods("POST"_method)([](const crow::request &req)
                                  { return Controllers::WebSocketController::broadcast(req); });

      CROW_ROUTE(app, "/api/ws/rooms/<string>/send")
          .CROW_MIDDLEWARES(app, Middlewares::JWTMiddleware)
          .methods("POST"_method)([](const crow::request &req, std::string room)
                                  { return Controllers::WebSocketController::sendToRoom(req, room); });
    }

  private:
    void registerCustomHandlers()
    {
      // Example: Custom "chat" event handler
      WebSocket::WebSocketManager::on("chat", [](crow::websocket::connection &conn, const nlohmann::json &payload)
                                      {
        std::string room = payload.value("room", "general");
        std::string message = payload.value("message", "");
        std::string sender = payload.value("sender", "anonymous");
        
        nlohmann::json chatMessage = {
          {"type", "chat"},
          {"room", room},
          {"sender", sender},
          {"message", message},
          {"timestamp", std::chrono::system_clock::now().time_since_epoch().count()}
        };
        
        WebSocket::WebSocketManager::sendToRoom(room, chatMessage); });

      // Example: Custom "typing" event handler
      WebSocket::WebSocketManager::on("typing", [](crow::websocket::connection &conn, const nlohmann::json &payload)
                                      {
        std::string room = payload.value("room", "general");
        std::string user = payload.value("user", "anonymous");
        bool isTyping = payload.value("isTyping", false);
        
        nlohmann::json typingNotification = {
          {"type", "typing"},
          {"room", room},
          {"user", user},
          {"isTyping", isTyping}
        };
        
        WebSocket::WebSocketManager::sendToRoom(room, typingNotification, &conn); });

      // Example: Custom "presence" event handler
      WebSocket::WebSocketManager::on("presence", [](crow::websocket::connection &conn, const nlohmann::json &payload)
                                      {
        std::string status = payload.value("status", "online");
        
        nlohmann::json info = WebSocket::WebSocketManager::getConnectionInfo(conn);
        
        nlohmann::json presenceUpdate = {
          {"type", "presence_update"},
          {"connectionId", info.value("connectionId", "")},
          {"status", status}
        };
        
        WebSocket::WebSocketManager::broadcast(presenceUpdate, &conn); });
    }
  };
}
