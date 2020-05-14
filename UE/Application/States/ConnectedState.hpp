#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);

    int lastSmsID = 1;

    void handleSmsReceived(common::PhoneNumber phoneNumber, std::string msg) final;

    void handleDisconnected() final;
    void handleSendMsg(common::PhoneNumber receiver, std::string content) final;

    void handleGetAllSmsBySent(bool) final;
    void handleGetSmsById(int id) final;
    void handleUpdateSms(Sms sms) final;
    void handleFailedToSendSms(int id) final;
    void handleCallRequest(common::PhoneNumber phoneNumber) final;
    void handleCallResponse(common::PhoneNumber phoneNumber, bool pass) final;
};

}
