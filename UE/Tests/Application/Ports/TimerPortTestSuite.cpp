#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Ports/TimerPort.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{
using namespace ::testing;

class TimerPortTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<ITimerEventsHandlerMock> handlerMock;

    TimerPort objectUnderTest{loggerMock};

    TimerPortTestSuite()
    {
        objectUnderTest.start(handlerMock);
    }
    ~TimerPortTestSuite()
    {
        objectUnderTest.stop();
    }
};

TEST_F(TimerPortTestSuite, shallStart)
{
}

TEST_F(TimerPortTestSuite, shallHandleTimeoutAfterPassedTime)
{
    using namespace std::chrono_literals;
    auto duration = 10ms;
    EXPECT_CALL(handlerMock, handleTimeout());
    objectUnderTest.startTimer(duration);
    std::this_thread::sleep_for(duration*2);
}

TEST_F(TimerPortTestSuite, shallStopTimer)
{
    using namespace std::chrono_literals;
    auto duration = 10ms;
    EXPECT_CALL(handlerMock, handleTimeout()).Times(0);
    objectUnderTest.startTimer(duration);
    objectUnderTest.stopTimer();
    std::this_thread::sleep_for(duration*2);
}

}
