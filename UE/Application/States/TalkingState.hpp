#pragma once

#include "BaseState.hpp"

namespace ue {

class TalkingState : public BaseState{
public:
    TalkingState(Context& context, const common::PhoneNumber from);

    void handleSendTalkMessage(const std::string) final;

private:
    common::PhoneNumber phone;
};

}
