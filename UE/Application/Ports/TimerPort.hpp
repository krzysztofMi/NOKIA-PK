#pragma once

#include <thread>
#include "ITimerPort.hpp"
#include "Logger/PrefixedLogger.hpp"

namespace ue
{

class TimerPort : public ITimerPort
{
public:
    TimerPort(common::ILogger& logger);

    void start(ITimerEventsHandler& handler);
    void stop();

    // ITimerPort interface
    void startTimer(const Duration duration) override;
    void stopTimer() override;

    void startCallTimer(const Duration duration, common::PhoneNumber phoneNumber) override;

private:
    common::PrefixedLogger logger;
    ITimerEventsHandler* handler = nullptr;
    std::thread timerThread;

    void waitForTimeout(const Duration duration) const;
    void waitForCallTimeout(const Duration duration, common::PhoneNumber phoneNumber) const;
};

}
