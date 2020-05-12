#pragma once

#include <chrono>
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class ITimerEventsHandler
{
public:
    virtual ~ITimerEventsHandler() = default;

    virtual void handleTimeout() = 0;
    virtual void handleCallTimeout(common::PhoneNumber phoneNumber) = 0;
};

class ITimerPort
{
public:
    using Duration = std::chrono::milliseconds;

    virtual ~ITimerPort() = default;

    virtual void startTimer(const Duration) = 0;
    virtual void startCallTimer(const Duration, common::PhoneNumber phoneNumber) = 0;
    virtual void stopTimer() = 0;
};

}
