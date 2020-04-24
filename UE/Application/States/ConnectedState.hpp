#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);

    void handleSmsReceived(common::PhoneNumber phoneNumber, std::string msg) final;

    void handleDisconnected() override;
    void handleSendMsg(common::PhoneNumber receiver, std::string content) final;

    void handleGetAllSmsBySent(bool) override;
    void handleGetSmsById(int id) override;
    virtual void handleUpdateSms(Sms sms) override;
};

}
