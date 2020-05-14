#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/ISmsDatabasePortMock.hpp"
#include <memory>

namespace ue
{
using namespace ::testing;

class ApplicationTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::PhoneNumber FROM{100};
    const common::BtsId BTS_ID{203};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IBtsPortMock> btsPortMock;
    StrictMock<IUserPortMock> userPortMock;
    StrictMock<ITimerPortMock> timerPortMock;
    StrictMock<ISmsDatabasePortMock> smsDatabasePortMock;

    Expectation notConnectedExpecation = EXPECT_CALL(userPortMock, showNotConnected());

    Application objectUnderTest{PHONE_NUMBER,
                                loggerMock,
                                btsPortMock,
                                userPortMock,
                                timerPortMock,
                                smsDatabasePortMock};
};

struct ApplicationNotConnectedTestSuite : ApplicationTestSuite
{};

TEST_F(ApplicationNotConnectedTestSuite, shallSetNotConnectedStateAtStartup)
{

}

struct ApplicationConnectingTestSuite : ApplicationNotConnectedTestSuite
{
  ApplicationConnectingTestSuite()
  {
      using namespace std::chrono_literals;
      EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
      EXPECT_CALL(timerPortMock, startTimer(500ms));
      EXPECT_CALL(userPortMock, showConnecting());
      objectUnderTest.handleSib(BTS_ID);
  }
};

TEST_F(ApplicationNotConnectedTestSuite, shallNotConnectedTestSuite)
{
   // implemented in constructor of test suite
}

TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnAttachReject)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    EXPECT_CALL(timerPortMock, stopTimer());
    objectUnderTest.handleAttachReject();
}

TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnTimeout)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleTimeout();
}

TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnDisconnect)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleDisconnected();
}

struct ApplicationConnectedTestSuite : ApplicationConnectingTestSuite
{
    ApplicationConnectedTestSuite()
    {
        EXPECT_CALL(userPortMock, showConnected());
        EXPECT_CALL(userPortMock, showMenuView());
        EXPECT_CALL(timerPortMock, stopTimer());
        objectUnderTest.handleAttachAccept();
    }
};

TEST_F(ApplicationConnectedTestSuite, shallShowConnectedOnAttachAccept)
{
  // Implemented in constructor of test suite
}

TEST_F(ApplicationConnectedTestSuite, shallShowNotConnectedOnDisconect){
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleDisconnected();
}

TEST_F(ApplicationConnectedTestSuite, shallSendMsg) {
    std::string message = "test";
    EXPECT_CALL(btsPortMock, sendMsg(FROM, message));
    EXPECT_CALL(smsDatabasePortMock, saveSms(message, FROM, true, true));
    objectUnderTest.handleSendMsg(FROM, message);
}

TEST_F(ApplicationConnectedTestSuite, shallSaveReceivedSms)
{
    auto message = "message";
    EXPECT_CALL(smsDatabasePortMock, saveSms(message, FROM, false, false));
    EXPECT_CALL(userPortMock, showReceivedSmsNotification);
    objectUnderTest.handleSmsReceived(FROM, message);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallRequest)
{
    using namespace std::chrono_literals;
    EXPECT_CALL(userPortMock, showRequestCallView(FROM));
    EXPECT_CALL(timerPortMock, startTimer(30000ms));
    objectUnderTest.handleCallRequest(FROM);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallReject)
{
    EXPECT_CALL(btsPortMock, sendCallResponse(FROM, false));
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showMenuView());
    objectUnderTest.handleCallResponse(FROM, false);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallTimeout)
{
    EXPECT_CALL(userPortMock, showMenuView());
    EXPECT_CALL(btsPortMock, sendCallResponse(FROM, false));
    objectUnderTest.handleTimeout();
}

struct ApplicationTalkingTestSuite : ApplicationConnectedTestSuite
{
    ApplicationTalkingTestSuite(){
        EXPECT_CALL(btsPortMock, sendCallResponse(FROM, true));
        EXPECT_CALL(timerPortMock, stopTimer());
        EXPECT_CALL(userPortMock, showCallView());
        objectUnderTest.handleCallResponse(FROM, true);
    }
};

TEST_F(ApplicationTalkingTestSuite, shallHandleCallAccept)
{
    //Implemented in ApplicationTalkingTestSuite constructor
}

}

