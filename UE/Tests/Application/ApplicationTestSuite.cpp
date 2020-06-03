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

    void changeToTalkingState(){
        EXPECT_CALL(timerPortMock, stopTimer());
        EXPECT_CALL(userPortMock, showCallView(""));
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
    EXPECT_CALL(btsPortMock, sendMsg(PHONE_NUMBER, message));
    EXPECT_CALL(smsDatabasePortMock, saveSms(message, PHONE_NUMBER, true, true));
    objectUnderTest.handleSendMsg(PHONE_NUMBER, message);
}

TEST_F(ApplicationConnectedTestSuite, shallFailedToSendSms) {
    std::string message = "test";
    EXPECT_CALL(smsDatabasePortMock, getSmsById(_));
    objectUnderTest.handleFailedToSendSms();
}

TEST_F(ApplicationConnectedTestSuite, shallSaveReceivedSms)
{
    auto message = "message";
    EXPECT_CALL(smsDatabasePortMock, saveSms(message, PHONE_NUMBER, false, false));
    EXPECT_CALL(userPortMock, showReceivedSmsNotification);
    objectUnderTest.handleSmsReceived(PHONE_NUMBER, message);
}

TEST_F(ApplicationConnectedTestSuite, shallSendCallRequest)
{
    using namespace std::chrono_literals;
    EXPECT_CALL(btsPortMock, sendCallRequest(PHONE_NUMBER));
    EXPECT_CALL(timerPortMock, startTimer(60000ms));
    EXPECT_CALL(userPortMock, showDialingView(PHONE_NUMBER));
    objectUnderTest.handleSendCallRequest(PHONE_NUMBER);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallRequest)
{
    using namespace std::chrono_literals;
    EXPECT_CALL(userPortMock, showRequestCallView(PHONE_NUMBER));
    EXPECT_CALL(timerPortMock, startTimer(30000ms));
    objectUnderTest.handleCallRequest(PHONE_NUMBER);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleSendCallReject)
{
    EXPECT_CALL(btsPortMock, sendCallResponse(PHONE_NUMBER, false));
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showMenuView());
    objectUnderTest.handleCallResponse(PHONE_NUMBER, false);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallAccept)
{
    changeToTalkingState();
    objectUnderTest.handleCallAccepted(PHONE_NUMBER);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallTimeout)
{
    EXPECT_CALL(userPortMock, showMenuView());
    EXPECT_CALL(btsPortMock, sendCallResponse(_, false));
    objectUnderTest.handleTimeout();
}


TEST_F(ApplicationConnectedTestSuite, shallHandlePeerUeBecomesUnknown)
{
    EXPECT_CALL(userPortMock, showPeerUeBecomesUnknown);
    EXPECT_CALL(userPortMock, showConnected);
    EXPECT_CALL(userPortMock, showMenuView);
    EXPECT_CALL(timerPortMock, stopTimer);
    objectUnderTest.handlePeerUeBecomesUnknown();
}

TEST_F(ApplicationConnectedTestSuite, handleCallDrop)
{
    EXPECT_CALL(timerPortMock, stopTimer);
    EXPECT_CALL(userPortMock, showMenuView);
    objectUnderTest.handleCallDrop();
}

struct ApplicationTalkingTestSuite : ApplicationConnectedTestSuite
{
    ApplicationTalkingTestSuite(){
        EXPECT_CALL(btsPortMock, sendCallResponse(PHONE_NUMBER, true));
        changeToTalkingState();
        objectUnderTest.handleCallResponse(PHONE_NUMBER, true);
    }

    void prepareTimer(){
        EXPECT_CALL(timerPortMock, stopTimer());
        using namespace std::chrono_literals;
        EXPECT_CALL(timerPortMock, startTimer(120000ms));
    }
};

TEST_F(ApplicationTalkingTestSuite, shallHandleCallAccept)
{
    //Implemented in ApplicationTalkingTestSuite constructor
}

TEST_F(ApplicationTalkingTestSuite, shallHandlePeerUeBecomesUnknown)
{
    EXPECT_CALL(userPortMock, showPeerUeBecomesUnknown);
    EXPECT_CALL(userPortMock, showConnected);
    EXPECT_CALL(userPortMock, showMenuView);
    EXPECT_CALL(timerPortMock, stopTimer);
    objectUnderTest.handlePeerUeBecomesUnknown();
}

TEST_F(ApplicationTalkingTestSuite, shallSendTalkMessage)
{
    const std::string message = "Two little cats.";
    prepareTimer();
    EXPECT_CALL(btsPortMock, sendTalkMessage(message, PHONE_NUMBER));
    objectUnderTest.handleSendTalkMessage(message);
}

TEST_F(ApplicationTalkingTestSuite, shallShowReceivedTalkMessage)
{
    prepareTimer();
    EXPECT_CALL(userPortMock, showCallView("Message"));
    objectUnderTest.handleTalkMessage("Message");
}


}

