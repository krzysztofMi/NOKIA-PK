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

}
