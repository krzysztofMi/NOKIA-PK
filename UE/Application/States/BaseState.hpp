#pragma once

#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Context.hpp"

namespace ue
{

class BaseState : public IEventsHandler
{
public:
    BaseState(Context& context, const std::string& name);
    ~BaseState() override;

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleDisconnected() override;
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleSmsReceived(common::PhoneNumber from, std::string message) override;
    void handleFailedToSendSms(int id) override;
    void handleCallRequest(common::PhoneNumber phoneNumber) override;

    // IUserEventsHandler interface
    void handleSendMsg(common::PhoneNumber receiver, std::string content) override;
    void handleGetAllSmsBySent(bool sent) override;
    void handleGetSmsById(int id) override;
    void handleUpdateSms(Sms sms) override;
    void handleCallResponse(common::PhoneNumber, bool pass) override;

protected:
    Context& context;
    common::PrefixedLogger logger;
};

}
