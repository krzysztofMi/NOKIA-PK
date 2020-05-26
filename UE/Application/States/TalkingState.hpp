#pragma once

#include "BaseState.hpp"

namespace ue {

class TalkingState : public BaseState{
public:
    TalkingState(Context& context, const common::PhoneNumber from);

private:
    common::PhoneNumber phone;
};

}
