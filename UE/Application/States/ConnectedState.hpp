#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
private:
    common::PhoneNumber currentlyDialedPhoneNumber;
    int lastSmsID;
public:
    ConnectedState(Context& context);

    void handleSmsReceived(common::PhoneNumber phoneNumber, std::string msg) final;

    void handleDisconnected() final;
    void handleSendMsg(common::PhoneNumber receiver, std::string content) final;

    void handleGetAllSmsBySent(bool) final;
    void handleGetSmsById(int id) final;
    void handleUpdateSms(Sms sms) final;
    void handleFailedToSendSms() final;
    void handleCallRequest(common::PhoneNumber phoneNumber) final;
    void handleCallResponse(common::PhoneNumber phoneNumber, bool pass) final;
    
    void handleTimeout() final;

    void handleCallAccepted(common::PhoneNumber from) final;
    void handleSendCallRequest(common::PhoneNumber to) final;

    void handleCallDrop() final;
};

}
