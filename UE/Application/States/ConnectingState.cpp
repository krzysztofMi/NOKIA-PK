#include "ConnectingState.hpp"
#include "NotConnectedState.hpp"
#include "ConnectedState.hpp"

namespace ue
{

ConnectingState::ConnectingState(Context &context, common::BtsId btsId)
    : BaseState(context, "ConnectingState")
{
    context.user.showConnecting();
    using namespace std::chrono_literals;
    context.bts.sendAttachRequest(btsId);
    context.timer.startTimer(500ms);
}

void ConnectingState::handleAttachReject(){
    context.logger.logError("attach Reject");
    context.timer.stopTimer();
    context.setState<NotConnectedState>();
}

void ConnectingState::handleTimeout(){
    context.logger.logError("timeout");
    context.setState<NotConnectedState>();
}

void ConnectingState::handleCallTimeout(common::PhoneNumber phoneNumber){
    context.logger.logError("timeout");
    context.user.showMenuView();
    context.bts.sendCallResponse(phoneNumber, 0);
}

void ConnectingState::handleAttachAccept()
{
    context.logger.logInfo("attach accept");
    context.timer.stopTimer();
    context.setState<ConnectedState>();
}

void ConnectingState::handleDisconnected()
{
    context.logger.logInfo("disconnecting");
    context.setState<NotConnectedState>();
}

}
