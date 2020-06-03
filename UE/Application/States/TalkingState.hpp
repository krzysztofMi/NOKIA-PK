#pragma once

#include "BaseState.hpp"


namespace ue {

class TalkingState : public BaseState{
public:
    TalkingState(Context& context, const common::PhoneNumber from);

    void handleSendTalkMessage(const std::string) final;
    void handleTalkMessage(const std::string) final;
    void handlePeerUeBecomesUnknown() final;
    void handleSendCallDrop(common::PhoneNumber sender, common::PhoneNumber receiver) final;
    void handleCallDrop() final;
private:
    common::PhoneNumber phone;
};

}
