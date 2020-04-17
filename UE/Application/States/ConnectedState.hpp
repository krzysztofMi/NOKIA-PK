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

};

}
