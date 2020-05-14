#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "TalkingState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
    context.user.showMenuView();
}

void ConnectedState::handleSmsReceived(common::PhoneNumber phoneNumber, 
                                       std::string msg)
{
    context.database.saveSms(msg, phoneNumber, false, false);
    context.user.showReceivedSmsNotification();
}

void ConnectedState::handleTimeout(){
    context.logger.logError("timeout");
    context.user.showMenuView();
    context.bts.sendCallResponse(this->currentlyDialedPhoneNumber, 0);
}

void ConnectedState::handleDisconnected(){
    logger.logInfo("disconnected");
    context.setState<NotConnectedState>();
}

void ConnectedState::handleSendMsg(common::PhoneNumber receiver, std::string content) {
    context.bts.sendMsg(receiver, content);
    context.database.saveSms(content, receiver, true, true);
}

void ConnectedState::handleGetAllSmsBySent(bool sent){
    context.user.showSmsListView(context.database.getAllSmsBySent(sent));
}

void ConnectedState::handleGetSmsById(int id){
    std::unique_ptr<Sms> sms = context.database.getSmsById(id);
    context.user.showSmsView(*sms);
}

void ConnectedState::handleUpdateSms(Sms sms){
    context.database.updateSms(sms);
}

void ConnectedState::handleCallRequest(common::PhoneNumber phoneNumber){
    using namespace std::chrono_literals;
    context.user.showRequestCallView(phoneNumber);
    this->currentlyDialedPhoneNumber = phoneNumber;
    context.timer.startTimer(30000ms);
}

void ConnectedState::handleCallResponse(common::PhoneNumber phoneNumber, bool pass){
    context.bts.sendCallResponse(phoneNumber, pass);
    if(pass){
        context.setState<TalkingState>();
    }else{
        context.user.showMenuView();
    }
    context.timer.stopTimer();
}

}
