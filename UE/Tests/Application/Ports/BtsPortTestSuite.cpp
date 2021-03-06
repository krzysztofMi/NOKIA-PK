#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Ports/BtsPort.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/ITransportMock.hpp"
#include "Messages/OutgoingMessage.hpp"
#include "Messages/IncomingMessage.hpp"

namespace ue
{
    using namespace ::testing;

    class BtsPortTestSuite : public Test
    {
    protected:
        const common::PhoneNumber PHONE_NUMBER{112};
        const common::BtsId BTS_ID{13121981ll};
        NiceMock<common::ILoggerMock> loggerMock;
        StrictMock<IBtsEventsHandlerMock> handlerMock;
        StrictMock<common::ITransportMock> transportMock;
        common::ITransport::MessageCallback messageCallback;
        common::ITransport::DisconnectedCallback disconnectedCallback;

        BtsPort objectUnderTest{loggerMock, transportMock, PHONE_NUMBER};

        BtsPortTestSuite()
        {
            EXPECT_CALL(transportMock, registerMessageCallback(_))
                .WillOnce(SaveArg<0>(&messageCallback));
            EXPECT_CALL(transportMock, registerDisconnectedCallback(_))
                .WillOnce(SaveArg<0>(&disconnectedCallback));
            objectUnderTest.start(handlerMock);
        }
        ~BtsPortTestSuite()
        {

            EXPECT_CALL(transportMock, registerMessageCallback(IsNull()));
            EXPECT_CALL(transportMock, registerDisconnectedCallback(IsNull()));
            objectUnderTest.stop();
        }
    };

    TEST_F(BtsPortTestSuite, shallRegisterHandlersBetweenStartStop)
    {
    }

    TEST_F(BtsPortTestSuite, shallHandleDiscconnected)
    {
        EXPECT_CALL(handlerMock, handleDisconnected());
        disconnectedCallback();
    }

    TEST_F(BtsPortTestSuite, shallIgnoreWrongMessage)
    {
        common::OutgoingMessage wrongMsg{};
        wrongMsg.writeBtsId(BTS_ID);
        messageCallback(wrongMsg.getMessage());
    }

    TEST_F(BtsPortTestSuite, shallHandleSib)
    {
        EXPECT_CALL(handlerMock, handleSib(BTS_ID));
        common::OutgoingMessage msg{common::MessageId::Sib,
                                    common::PhoneNumber{},
                                    PHONE_NUMBER};
        msg.writeBtsId(BTS_ID);
        messageCallback(msg.getMessage());
    }

    TEST_F(BtsPortTestSuite, shallHandleAttachAccept)
    {
        EXPECT_CALL(handlerMock, handleAttachAccept());
        common::OutgoingMessage msg{common::MessageId::AttachResponse,
                                    common::PhoneNumber{},
                                    PHONE_NUMBER};
        msg.writeNumber(true);
        messageCallback(msg.getMessage());
    }

    TEST_F(BtsPortTestSuite, shallHandleAttachReject)
    {
        EXPECT_CALL(handlerMock, handleAttachReject());
        common::OutgoingMessage msg{common::MessageId::AttachResponse,
                                    common::PhoneNumber{},
                                    PHONE_NUMBER};
        msg.writeNumber(false);
        messageCallback(msg.getMessage());
    }

    TEST_F(BtsPortTestSuite, shallSendAttachRequest)
    {
        common::BinaryMessage msg;
        EXPECT_CALL(transportMock, sendMessage(_)).WillOnce(SaveArg<0>(&msg));
        objectUnderTest.sendAttachRequest(BTS_ID);
        common::IncomingMessage reader(msg);
        ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::AttachRequest, reader.readMessageId()));
        ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
        ASSERT_NO_THROW(EXPECT_EQ(common::PhoneNumber{}, reader.readPhoneNumber()));
        ASSERT_NO_THROW(EXPECT_EQ(BTS_ID, reader.readBtsId()));
        ASSERT_NO_THROW(reader.checkEndOfMessage());
    }

    TEST_F(BtsPortTestSuite, shallHandleReceivedSms)
    {
        EXPECT_CALL(handlerMock, handleSmsReceived);
        common::OutgoingMessage message{common::MessageId::Sms,
                                        common::PhoneNumber{},
                                        PHONE_NUMBER};
        messageCallback(message.getMessage());
    }

    TEST_F(BtsPortTestSuite, shallSendMsg)
    {
        common::BinaryMessage msg;
        EXPECT_CALL(transportMock, sendMessage(_)).WillOnce(SaveArg<0>(&msg));
        objectUnderTest.sendMsg(common::PhoneNumber{115}, "test");
        common::IncomingMessage reader(msg);
        ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::Sms, reader.readMessageId()));
        ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
        ASSERT_NO_THROW(EXPECT_EQ(common::PhoneNumber{115}, reader.readPhoneNumber()));
        ASSERT_NO_THROW(EXPECT_EQ("test", reader.readRemainingText()));
        ASSERT_NO_THROW(reader.checkEndOfMessage());
    }

    TEST_F(BtsPortTestSuite, shallHandleCallRequest)
    {
        auto phoneNumber = common::PhoneNumber{100};
        EXPECT_CALL(handlerMock, handleCallRequest(phoneNumber));
        common::OutgoingMessage message{common::MessageId::CallRequest,
                                        phoneNumber,
                                        PHONE_NUMBER};
        messageCallback(message.getMessage());
    }

    TEST_F(BtsPortTestSuite, shallHandleCallAccept)
    {
        auto phoneNumber = common::PhoneNumber{100};
        EXPECT_CALL(handlerMock, handleCallAccepted(phoneNumber));
        common::OutgoingMessage message{common::MessageId::CallAccepted,
                                        common::PhoneNumber{100},
                                        PHONE_NUMBER};
        messageCallback(message.getMessage());
    }

    TEST_F(BtsPortTestSuite, shallHandlePeerUeBecomesUnknownCallAccepted)
    {
        EXPECT_CALL(handlerMock, handlePeerUeBecomesUnknown());
        common::OutgoingMessage message{common::MessageId::UnknownRecipient,
                                        common::PhoneNumber{},
                                        PHONE_NUMBER};
        message.writeMessageId(common::MessageId::CallAccepted);
        messageCallback(message.getMessage());
    }

    TEST_F(BtsPortTestSuite, shallHandlePeerUeBecomesUnknownCallDroped)
    {
        common::OutgoingMessage message{common::MessageId::UnknownRecipient,
                                        common::PhoneNumber{},
                                        PHONE_NUMBER};
        message.writeMessageId(common::MessageId::CallDropped);
        messageCallback(message.getMessage());
    }

    TEST_F(BtsPortTestSuite, shallHandlePeerUeBecomesUnknownSmsMessage)
    {
        EXPECT_CALL(handlerMock, handleFailedToSendSms());
        common::OutgoingMessage message{common::MessageId::UnknownRecipient,
                                        common::PhoneNumber{},
                                        PHONE_NUMBER};
        message.writeMessageId(common::MessageId::Sms);
        messageCallback(message.getMessage());
    }

    TEST_F(BtsPortTestSuite, shallHandlePeerUeBecomesUnknownDisconnected)
    {
        EXPECT_CALL(handlerMock, handlePeerUeBecomesUnknown());
        common::OutgoingMessage message{common::MessageId::UnknownRecipient,
                                        common::PhoneNumber{},
                                        PHONE_NUMBER};
        message.writeMessageId(common::MessageId::UnknownRecipient);
        messageCallback(message.getMessage());
    }

    TEST_F(BtsPortTestSuite, shallSendAcceptCallResponse)
    {
        auto receiver = common::PhoneNumber{100};
        common::BinaryMessage msg;
        EXPECT_CALL(transportMock, sendMessage(_)).WillOnce(SaveArg<0>(&msg));
        objectUnderTest.sendCallResponse(receiver, true);
        common::IncomingMessage reader(msg);
        ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::CallAccepted, reader.readMessageId()));
        ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
        ASSERT_NO_THROW(EXPECT_EQ(receiver, reader.readPhoneNumber()));
        ASSERT_NO_THROW(reader.checkEndOfMessage());
    }

    TEST_F(BtsPortTestSuite, shallSendRejectCallResponse)
    {
        auto receiver = common::PhoneNumber{100};
        common::BinaryMessage msg;
        EXPECT_CALL(transportMock, sendMessage(_)).WillOnce(SaveArg<0>(&msg));
        objectUnderTest.sendCallResponse(receiver, false);
        common::IncomingMessage reader(msg);
        ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::CallDropped, reader.readMessageId()));
        ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
        ASSERT_NO_THROW(EXPECT_EQ(receiver, reader.readPhoneNumber()));
        ASSERT_NO_THROW(reader.checkEndOfMessage());
    }

    TEST_F(BtsPortTestSuite, shallSendCallRequest)
    {
        auto to = common::PhoneNumber{100};
        common::BinaryMessage msg;
        EXPECT_CALL(transportMock, sendMessage(_)).WillOnce(SaveArg<0>(&msg));
        objectUnderTest.sendCallRequest(to);
        common::IncomingMessage reader(msg);
        ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::CallRequest, reader.readMessageId()));
        ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
        ASSERT_NO_THROW(EXPECT_EQ(to, reader.readPhoneNumber()));
        ASSERT_NO_THROW(reader.checkEndOfMessage());
    }

    TEST_F(BtsPortTestSuite, shallSendTalkMessage)
    {
        auto to = common::PhoneNumber{100};
        common::BinaryMessage msg;
        EXPECT_CALL(transportMock, sendMessage(_)).WillOnce(SaveArg<0>(&msg));
        objectUnderTest.sendTalkMessage("Hello world.", to);
        common::IncomingMessage reader(msg);
        ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::CallTalk, reader.readMessageId()));
        ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
        ASSERT_NO_THROW(EXPECT_EQ(to, reader.readPhoneNumber()));
        ASSERT_NO_THROW(EXPECT_EQ("Hello world.", reader.readRemainingText()));
        ASSERT_NO_THROW(reader.checkEndOfMessage());
    }

    TEST_F(BtsPortTestSuite, shallReceiveCallTalk)
    {
        auto phoneNumber = common::PhoneNumber{100};
        const std::string text{"Ala ma kota"};
        EXPECT_CALL(handlerMock, handleTalkMessage(text));
        common::OutgoingMessage message{common::MessageId::CallTalk,
                                        common::PhoneNumber{100},
                                        PHONE_NUMBER};
        message.writeText("Ala ma kota");
        messageCallback(message.getMessage());
    }

    TEST_F(BtsPortTestSuite, shallCallDropped )
    {
        auto phoneNumber = common::PhoneNumber{100};
        EXPECT_CALL(handlerMock, handleCallDrop());
        common::OutgoingMessage message{common::MessageId::CallDropped,
                                        common::PhoneNumber{100},
                                        PHONE_NUMBER};
        messageCallback(message.getMessage());
    }

    TEST_F(BtsPortTestSuite, shallCallDroppedDuringEstablishedConnection )
    {
        common::BinaryMessage msg;
        EXPECT_CALL(transportMock, sendMessage(_)).WillOnce(SaveArg<0>(&msg));
        objectUnderTest.sendCallDrop(PHONE_NUMBER, common::PhoneNumber{120});
        common::IncomingMessage reader(msg);
        ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::CallDropped, reader.readMessageId()));
        ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
        ASSERT_NO_THROW(EXPECT_EQ(common::PhoneNumber{120}, reader.readPhoneNumber()));
        ASSERT_NO_THROW(reader.checkEndOfMessage());
    }

} // namespace ue


