#pragma once

#include <chrono>

namespace ue
{

class ITimerEventsHandler
{
public:
    virtual ~ITimerEventsHandler() = default;

    virtual void handleTimeout(common::PhoneNumber phoneNumber) = 0;
};

class ITimerPort
{
public:
    using Duration = std::chrono::milliseconds;

    virtual ~ITimerPort() = default;

    virtual void startTimer(const Duration, common::PhoneNumber phoneNumber) = 0;
    virtual void stopTimer() = 0;
};

}
