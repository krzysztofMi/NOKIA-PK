#include "TimerPort.hpp"

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
    running = true;
    timerThread = std::thread {&TimerPort::waitForTimeout, this, duration};
}

void TimerPort::stopTimer()
{
    running = false;
    timerThread.detach();
    logger.logDebug("Stop timer");
}

void TimerPort::waitForTimeout(Duration duration) const
{
    std::this_thread::sleep_for(duration);
    if(!running) { return; }
    handler->handleTimeout();
}

}
