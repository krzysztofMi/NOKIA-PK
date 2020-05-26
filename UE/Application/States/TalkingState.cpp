#include "TalkingState.hpp"

namespace ue {

TalkingState::TalkingState(Context& context, const common::PhoneNumber from)
    : BaseState(context, "TalkingState"), phone{from}
{
    context.user.showCallView("");
}

void TalkingState::handleSendTalkMessage(const std::string message){
    context.timer.stopTimer();
    using namespace std::chrono_literals;
    context.timer.startTimer(120000ms);
    context.bts.sendTalkMessage(message, phone);
}

void TalkingState::handleTalkMessage(const std::string message){
    context.timer.stopTimer();
    using namespace std::chrono_literals;
    context.timer.startTimer(120000ms);
    context.user.showCallView(message);
}
}
