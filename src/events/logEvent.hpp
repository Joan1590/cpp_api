#include <iostream>
#include <thread>
#include "../redis/RedisManager.hpp"

void logEvent()
{
  while (true)
  {
    try
    {
      RedisManager manager; // Create manager inside the thread
      auto *redis = manager.getRedis();

      // Create a subscriber
      auto sub = redis->subscriber();

      // Subscribe to the "log" channel
      sub.subscribe("log");

      // Set the callback function for receiving messages
      sub.on_message([](std::string channel, std::string msg)
                     { std::cout << "Received message from " << channel << ": " << msg << std::endl; });

      // Consume messages in a loop
      while (true)
      {
        sub.consume();
      }
    }
    catch (const sw::redis::Error &e)
    {
    }
  }
}