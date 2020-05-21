#include "TalkingState.hpp"

namespace ue {
    TalkingState::TalkingState(Context& context, const common::PhoneNumber from)
        : BaseState(context, "TalkingState"), phone{from}
    {
        context.user.showCallView();
    }
}
