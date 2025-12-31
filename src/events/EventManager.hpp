#pragma once
#include "logEvent.hpp"

namespace Events
{
  class EventManager
  {
  private:
    LogEventSubscriber logSubscriber_;

  public:
    void start()
    {
      logSubscriber_.start();
    }

    void stop()
    {
      logSubscriber_.stop();
    }

    bool isRunning() const
    {
      return logSubscriber_.isRunning();
    }
  };
}