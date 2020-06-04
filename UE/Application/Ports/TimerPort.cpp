#include "TimerPort.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

std::mutex TimerPort::mutex;
std::condition_variable TimerPort::cv;

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
}

void TimerPort::startTimer(const Duration duration)
{
    logger.logDebug("Start timer: ", duration.count(), "ms");
    flag = false;
    timerThread = std::thread {&TimerPort::waitForTimeout, this, duration};
    timerThread.detach();
}

void TimerPort::stopTimer()
{
    {
        std::lock_guard<std::mutex> lock(mutex);
        flag = true;
    }
    cv.notify_one();
    logger.logDebug("Stop timer");
}

void TimerPort::waitForTimeout(Duration duration) const 
{
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait_for(lock, duration, [this](){return flag;});
    if(!flag){
        handler->handleTimeout();    
    }
    return;
}

}
