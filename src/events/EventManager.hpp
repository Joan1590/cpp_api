#pragma once
#include "logEvent.hpp"
#include <thread>
#include "../redis/RedisManager.hpp"

namespace Events
{
  class EventManager
  {
  public:
    void runThreads()
    {
      RedisManager manager;
      std::thread sub_thread(logEvent);

      sub_thread.detach();
    }
  };

}