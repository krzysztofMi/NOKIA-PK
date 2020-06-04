#pragma once

#include <thread>
#include <condition_variable>
#include <atomic>
#include <mutex>
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
   
private:
    common::PrefixedLogger logger;
    ITimerEventsHandler* handler = nullptr;
    std::thread timerThread;
    static std::mutex mutex;
    bool flag;
    static std::condition_variable cv;
    void waitForTimeout(const Duration duration) const;
};

}
