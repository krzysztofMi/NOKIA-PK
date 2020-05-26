#pragma once

#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"
#include "IEventsHandler.hpp"
#include "Context.hpp"

namespace ue
{

using common::PhoneNumber;
using common::ILogger;

class Application : public IEventsHandler
{
public:
    Application(PhoneNumber phoneNumber,
                ILogger& iLogger,
                IBtsPort& bts,
                IUserPort& user,
                ITimerPort& timer,
                ISmsDatabasePort& database);
    ~Application();

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleDisconnected() override;
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleSmsReceived(common::PhoneNumber PhoneNumber, std::string msg) override;
    void handleCallRequest(common::PhoneNumber phoneNumber) override;
    void handleCallAccepted(common::PhoneNumber PhoneNumber) override;
    void handleFailedToSendSms() override;

    // IUserPortHandler interface
    void handleSendMsg(common::PhoneNumber receiver, std::string content) override;
    void handleGetSmsById(int id) override;
    void handleGetAllSmsBySent(bool sent) override;
    void handleUpdateSms(Sms sms) override;
    void handleCallResponse(common::PhoneNumber, bool pass) override;
    void handleSendCallRequest(common::PhoneNumber to);

private:
    Context context;
    common::PrefixedLogger logger;

};

}
