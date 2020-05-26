#pragma once

#include "BaseState.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class ConnectingState : public BaseState
{
public:
    ConnectingState(Context& context, common::BtsId btsId);
    void handleAttachReject() override;
    void handleTimeout() override;
    void handleAttachAccept() override;
    void handleDisconnected() override;
};

}
