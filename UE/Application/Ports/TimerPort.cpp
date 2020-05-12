#include "TimerPort.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

TimerPort::TimerPort(common::ILogger &logger)
    : logger(logger, "[TIMER PORT]")
{}

void TimerPort::start(ITimerEventsHandler &handler)
{
    logger.logDebug("Started");
    this->handler = &handler;
}

void TimerPort::stop()
{
    logger.logDebug("Stoped");
    handler = nullptr;
    if(timerThread.joinable()){
        timerThread.detach();
    }
}

void TimerPort::startTimer(const Duration duration)
{
    logger.logDebug("Start timer: ", duration.count(), "ms");
    timerThread = std::thread {&TimerPort::waitForTimeout, this, duration};
}

void TimerPort::startCallTimer(const Duration duration, common::PhoneNumber phoneNumber)
{
    logger.logDebug("Start timer: ", duration.count(), "ms");
    timerThread = std::thread {&TimerPort::waitForCallTimeout, this, duration, phoneNumber};
}

void TimerPort::stopTimer()
{
    pthread_cancel(timerThread.native_handle());
    logger.logDebug("Stop timer");
}

void TimerPort::waitForTimeout(Duration duration) const
{
    std::this_thread::sleep_for(duration);
    handler->handleTimeout();
}

void TimerPort::waitForCallTimeout(Duration duration, common::PhoneNumber phoneNumber) const
{
    std::this_thread::sleep_for(duration);
    handler->handleCallTimeout(phoneNumber);
}

}
