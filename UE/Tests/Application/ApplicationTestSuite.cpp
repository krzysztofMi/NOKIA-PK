#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/IOrmMock.hpp"
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
    StrictMock<IOrmMock<Sms>> smsOrmMock;

    Expectation notConnectedExpecation = EXPECT_CALL(userPortMock, showNotConnected());

    Application objectUnderTest{PHONE_NUMBER,
                                loggerMock,
                                btsPortMock,
                                userPortMock,
                                timerPortMock,
                                smsOrmMock};
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
    EXPECT_CALL(btsPortMock, sendMsg(common::PhoneNumber{115}, "test"));
    EXPECT_CALL(smsOrmMock,insert(_));
    objectUnderTest.handleSendMsg(common::PhoneNumber{115}, "test");
}

TEST_F(ApplicationConnectedTestSuite, shallSaveReceivedSms)
{
    auto phoneNumber = common::PhoneNumber{111};
    auto message = "message";
    Sms incomingSms = Sms {
            phoneNumber.value,
            message,
            false,
            false
        };


    EXPECT_CALL(smsOrmMock, insert(_));

    objectUnderTest.handleSmsReceived(phoneNumber, message);
}


}
