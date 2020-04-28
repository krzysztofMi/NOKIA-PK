#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);

    void handleSmsReceived(common::PhoneNumber phoneNumber, std::string msg) final;

    void handleDisconnected() final;
    void handleSendMsg(common::PhoneNumber receiver, std::string content) final;

    void handleGetAllSmsBySent(bool) final;
    void handleGetSmsById(int id) final;
    void handleUpdateSms(Sms sms) final;
};

}
