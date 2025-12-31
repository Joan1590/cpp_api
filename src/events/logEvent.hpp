#pragma once

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include "../redis/RedisManager.hpp"

class LogEventSubscriber
{
private:
  std::atomic<bool> running_{false};
  std::thread worker_;
  static constexpr int MAX_RECONNECT_DELAY_MS = 30000;
  static constexpr int INITIAL_RECONNECT_DELAY_MS = 1000;

  void run()
  {
    int reconnectDelay = INITIAL_RECONNECT_DELAY_MS;

    while (running_.load())
    {
      try
      {
        RedisManager manager;
        auto *redis = manager.getRedis();
        auto sub = redis->subscriber();

        sub.subscribe("log");

        sub.on_message([](std::string channel, std::string msg)
                       { std::cout << "[Redis] " << channel << ": " << msg << std::endl; });

        // Reset delay on successful connection
        reconnectDelay = INITIAL_RECONNECT_DELAY_MS;
        std::cout << "[LogEvent] Subscribed to 'log' channel" << std::endl;

        // Use consume with timeout to allow checking running_ flag
        while (running_.load())
        {
          try
          {
            sub.consume();
          }
          catch (const sw::redis::TimeoutError &)
          {
            // Timeout is expected, continue loop to check running_ flag
          }
        }
      }
      catch (const sw::redis::Error &e)
      {
        if (running_.load())
        {
          std::cerr << "[LogEvent] Redis error: " << e.what()
                    << ". Reconnecting in " << reconnectDelay << "ms..." << std::endl;

          std::this_thread::sleep_for(std::chrono::milliseconds(reconnectDelay));

          // Exponential backoff with max limit
          reconnectDelay = std::min(reconnectDelay * 2, MAX_RECONNECT_DELAY_MS);
        }
      }
    }
  }

public:
  void start()
  {
    if (!running_.load())
    {
      running_.store(true);
      worker_ = std::thread(&LogEventSubscriber::run, this);
    }
  }

  void stop()
  {
    running_.store(false);
    if (worker_.joinable())
    {
      worker_.join();
    }
  }

  bool isRunning() const { return running_.load(); }

  ~LogEventSubscriber()
  {
    stop();
  }
};