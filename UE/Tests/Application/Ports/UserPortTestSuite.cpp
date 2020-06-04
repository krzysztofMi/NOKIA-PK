#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Ports/UserPort.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/IUeGuiMock.hpp"
#include "Mocks/ISmsDatabasePortMock.hpp"

namespace ue
{
using namespace ::testing;

class UserPortTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IUserEventsHandlerMock> handlerMock;
    StrictMock<IUeGuiMock> guiMock;
    StrictMock<IListViewModeMock> listViewModeMock;
    StrictMock<ITextModeMock> textViewModeMock;
    StrictMock<IDialModeMock> dialModeMock;
    StrictMock<ICallModeMock> callModeMock;
    StrictMock<ISmsDatabasePortMock> smsDatabaseMock;
    UserPort objectUnderTest{loggerMock, guiMock, PHONE_NUMBER};

    UserPortTestSuite()
    {
        EXPECT_CALL(guiMock, setTitle(HasSubstr(to_string(PHONE_NUMBER))));
        objectUnderTest.start(handlerMock);
    }
    ~UserPortTestSuite()
    {
        objectUnderTest.stop();
    }
};

TEST_F(UserPortTestSuite, shallStartStop)
{
}

TEST_F(UserPortTestSuite, shallShowNotConnected)
{
    EXPECT_CALL(guiMock, showNotConnected());
    objectUnderTest.showNotConnected();
}

TEST_F(UserPortTestSuite, shallShowConnecting)
{
    EXPECT_CALL(guiMock, showConnecting());
    objectUnderTest.showConnecting();
}

TEST_F(UserPortTestSuite, shallShowConnected)
{
    EXPECT_CALL(guiMock, showConnected());
    objectUnderTest.showConnected();
}

TEST_F(UserPortTestSuite, shallShowMenu)
{
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));
    EXPECT_CALL(guiMock, setAcceptCallback);
    objectUnderTest.showMenuView();
}

TEST_F(UserPortTestSuite, shallShowRequestCallView)
{
    auto phoneNumber = common::PhoneNumber{100};
    EXPECT_CALL(guiMock, setAlertMode()).WillOnce(ReturnRef(textViewModeMock));
    EXPECT_CALL(textViewModeMock, setText("New call from\n100"));
    EXPECT_CALL(guiMock, setAcceptCallback);
    EXPECT_CALL(guiMock, setRejectCallback);
    objectUnderTest.showRequestCallView(phoneNumber);
}

TEST_F(UserPortTestSuite, shallShowStartDialView)
{
    EXPECT_CALL(guiMock, setDialMode()).WillOnce(ReturnRef(dialModeMock));
    EXPECT_CALL(guiMock, setAcceptCallback);
    EXPECT_CALL(guiMock, setRejectCallback);
    objectUnderTest.showStartDialView();
}

TEST_F(UserPortTestSuite, shallShowDialingView)
{
    auto phoneNumber = common::PhoneNumber{100};
    EXPECT_CALL(guiMock, setAlertMode()).WillOnce(ReturnRef(textViewModeMock));
    EXPECT_CALL(textViewModeMock, setText("Waiting for call accept..."));
    EXPECT_CALL(guiMock, setAcceptCallback);
    EXPECT_CALL(guiMock, setRejectCallback);
    objectUnderTest.showDialingView(phoneNumber);
}

TEST_F(UserPortTestSuite, shallShowCallView)
{
    EXPECT_CALL(guiMock, setCallMode()).WillOnce(ReturnRef(callModeMock));
    EXPECT_CALL(callModeMock, appendIncomingText("aaa"));
    EXPECT_CALL(guiMock, setAcceptCallback);
    EXPECT_CALL(guiMock, setRejectCallback);
    objectUnderTest.showCallView("aaa");
}

TEST_F(UserPortTestSuite, shallClearCallMessage)
{
    EXPECT_CALL(guiMock, setCallMode()).WillOnce(ReturnRef(callModeMock));
    EXPECT_CALL(callModeMock, clearIncomingText());    
    objectUnderTest.clearCallMessages();
}
}
