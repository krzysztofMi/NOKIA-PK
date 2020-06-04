#include "TalkingState.hpp"
#include "ConnectedState.hpp"

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

void TalkingState::handlePeerUeBecomesUnknown()
{
    context.timer.stopTimer();
    context.user.showPeerUeBecomesUnknown(this->phone);
    context.setState<ConnectedState>();
}

void TalkingState::handleSendCallDrop(common::PhoneNumber sender, common::PhoneNumber receiver)
{
    context.timer.stopTimer();
    context.bts.sendCallDrop(sender,receiver);
    context.user.clearCallMessages();
    context.setState<ConnectedState>();
}

void TalkingState::handleCallDrop()
{
    context.timer.stopTimer();
    context.user.clearCallMessages();
    context.setState<ConnectedState>();
}

void TalkingState::handleTimeout()
{
    context.bts.sendCallResponse(phone, false);
    context.setState<ConnectedState>();
}
}
