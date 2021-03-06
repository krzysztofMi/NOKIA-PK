#include "BtsPort.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"

namespace ue
{

BtsPort::BtsPort(common::ILogger &logger, common::ITransport &transport, common::PhoneNumber phoneNumber)
    : logger(logger, "[BTS-PORT]"),
        transport(transport),
        phoneNumber(phoneNumber)
{
}

void BtsPort::start(IBtsEventsHandler &handler)
{
    transport.registerMessageCallback([this](BinaryMessage msg) { handleMessage(msg); });
    transport.registerDisconnectedCallback([this] { handleDisconnected(); });
    this->handler = &handler;
}

void BtsPort::stop()
{
    transport.registerMessageCallback(nullptr);
    transport.registerDisconnectedCallback(nullptr);
    handler = nullptr;
}

void BtsPort::handleDisconnected()
{
    logger.logError("disconnected");
    handler->handleDisconnected();
}

void BtsPort::handleMessage(BinaryMessage msg)
{
    try
    {
        common::IncomingMessage reader{msg};
        auto messageId = reader.readMessageId();
        common::PhoneNumber from = reader.readPhoneNumber();
        auto to = reader.readPhoneNumber();

        switch (messageId)
        {
        case common::MessageId::Sib:
        {
            auto btsId = reader.readBtsId();
            handler->handleSib(btsId);
            break;
        }
        case common::MessageId::AttachResponse:
        {
            bool accept = reader.readNumber<std::uint8_t>() != 0u;
            if (accept)
                handler->handleAttachAccept();
            else
                handler->handleAttachReject();
            break;
        }
        case common::MessageId::Sms:
        {
            std::string message = reader.readRemainingText();
            logger.logError("BtsPort, SmsRecieved from: ", from);
            logger.logError("BtsPort, SmsRecieved msg: ", message);
            handler->handleSmsReceived(from, message);
            break;
        }
        case common::MessageId::CallRequest:
        {
            logger.logError("BtsPort, Call request from:", from);
            handler->handleCallRequest(from);
            break;
        }
        case common::MessageId::CallAccepted:
        {
            logger.logError("BtsPort, Call accepted from", from);
            handler->handleCallAccepted(from);
            break;
        }
        case common::MessageId::CallTalk:
        {
            logger.logError("BtsPort, Call talk", from);
            handler->handleTalkMessage(reader.readRemainingText());
            break;
        }
        case common::MessageId::UnknownRecipient:
        {
            logger.logError("Unknown recipient ");
            switch (reader.readMessageId())
            {
                case common::MessageId::Sms:
                {
                    logger.logError("common::MessageId::Sms");
                    handler->handleFailedToSendSms();
                    break;
                }
                case common::MessageId::CallAccepted:
                {
                    logger.logError("common::MessageId::CallAccepted");
                    handler->handlePeerUeBecomesUnknown();
                    break;
                }
                case common::MessageId::CallDropped:
                {
                    logger.logError("common::MessageId::CallDropped");
                    // Ignore message
                    // so that it basically hangs
                    break;
                }
                default:
                {
                    // UE disconnected
                    logger.logError("common::MessageId::*");
                    handler->handlePeerUeBecomesUnknown();
                    break;
                }
            }
            break;
        }
        case common::MessageId::CallDropped:
        {
            logger.logError("BtsPort, Call drop from ", from);
            handler->handleCallDrop();
            break;
        }
        default:
            logger.logError("Unknown message: ", messageId, ", from: ", from);
        }
    }
    catch (std::exception const &ex)
    {
        logger.logError("handleMessage error: ", ex.what());
    }
}

void BtsPort::sendAttachRequest(common::BtsId btsId)
{
    logger.logDebug("sendAttachRequest: ", btsId);
    common::OutgoingMessage msg{common::MessageId::AttachRequest,
                                phoneNumber,
                                common::PhoneNumber{}};
    msg.writeBtsId(btsId);
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendMsg(common::PhoneNumber receiver, std::string content)
{
    logger.logDebug("sendSms: ", receiver);
    common::OutgoingMessage msg{
        common::MessageId::Sms, phoneNumber, receiver};
    msg.writeText(content);
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallResponse(common::PhoneNumber receiver, bool pass)
{
    logger.logDebug("Send call response:", receiver, pass);
    auto messageId = pass ? common::MessageId::CallAccepted : common::MessageId::CallDropped;
    common::OutgoingMessage msg{
        messageId, phoneNumber, receiver};
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallRequest(common::PhoneNumber to)
{
    logger.logDebug("Send call request:", to);
    common::OutgoingMessage msg{
        common::MessageId::CallRequest, phoneNumber, to};
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendTalkMessage(const std::string message, const common::PhoneNumber to)
{
    logger.logDebug("Send talk message", message, to);
    common::OutgoingMessage msg{
        common::MessageId::CallTalk, phoneNumber, to};
    msg.writeText(message);
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallDrop(common::PhoneNumber sender, common::PhoneNumber receiver)
{
    logger.logDebug("Send call dropped: ", receiver);
    common::OutgoingMessage msg{
        common::MessageId::CallDropped, phoneNumber, receiver};
    transport.sendMessage(msg.getMessage());
}
} // namespace ue
