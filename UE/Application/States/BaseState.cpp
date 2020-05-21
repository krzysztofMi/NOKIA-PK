#include "BaseState.hpp"

namespace ue
{

BaseState::BaseState(Context &context, const std::string &name)
    : context(context),
      logger(context.logger, "[" + name + "]")
{
    logger.logDebug("entry");
}

BaseState::~BaseState()
{
    logger.logDebug("exit");
}

void BaseState::handleTimeout()
{
    logger.logError("Uexpected: handleTimeout");
}

void BaseState::handleDisconnected()
{
    logger.logError("Uexpected: handleDisconnected()");
}

void BaseState::handleSib(common::BtsId btsId)
{
    logger.logError("Uexpected: handleSib: ", btsId);
}

void BaseState::handleAttachAccept()
{
    logger.logError("Uexpected: handleAttachAccept");
}

void BaseState::handleAttachReject()
{
    logger.logError("Uexpected: handleAttachReject");
}

void BaseState::handleSmsReceived(common::PhoneNumber from, std::string message)
{
    logger.logError("Unexpected handleSmsReceived", from, message);
}

void BaseState::handleSendMsg(common::PhoneNumber receiver, std::string content)
{
    logger.logError("Unexpected handleSendMsg", receiver, content);
}

void BaseState::handleFailedToSendSms()
{
    logger.logError("Unexpected handelFailedToSendSms");
}

void BaseState::handleGetAllSmsBySent(bool sent)
{
    logger.logError("Unexpected handleGetAllSmsBySent", sent);
}

void BaseState::handleGetSmsById(int id)
{
    logger.logError("Unexpected handeGetSmsById", id);
}

void BaseState::handleUpdateSms(Sms sms)
{
    logger.logError("Unexpected handleUpdateSms", sms.phoneNumber, sms.text);
}

void BaseState::handleCallRequest(common::PhoneNumber phoneNumber)
{
    logger.logError("Unexpected handleCallRequest", phoneNumber);
}

void BaseState::handleCallResponse(common::PhoneNumber phoneNumber, bool pass)
{
    logger.logError("Unexpected handleCallResponse", phoneNumber, pass);
}

void BaseState::handleCallAccepted(common::PhoneNumber phone)
{
    logger.logError("Unexpected handleCallAccepted", phone);
}

void BaseState::handleSendCallRequest(common::PhoneNumber to)
{
    logger.logError("Unexpected handleSendCallRequest", to);
}

void BaseState::handleSendTalkMessage(const std::string message)
{
    logger.logError("Unexpected handleSendTalkMessage", message);
}

void BaseState::handleTalkMessage(const std::string message)
{
    logger.logError("Unexpected handle talk message", message);
}

}
