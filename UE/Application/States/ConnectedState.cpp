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

void ConnectedState::handleFailedToSendSms()
{
    std::unique_ptr<Sms> sms = context.database.getSmsById(lastSmsID);
    if(sms!=nullptr) {
        sms->failed = true;
        context.database.updateSms(*sms.get());
    }
}

void ConnectedState::handleDisconnected(){
    logger.logInfo("disconnected");
    context.setState<NotConnectedState>();
}

void ConnectedState::handleSendMsg(common::PhoneNumber receiver, std::string content) {
    context.bts.sendMsg(receiver, content);
    Sms lastSms = context.database.saveSms(content, receiver, true, true);
    lastSmsID = lastSms.id;
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
    this->currentlyDialedPhoneNumber = phoneNumber;
    context.user.showRequestCallView(phoneNumber);
    context.timer.startTimer(30000ms);
}

void ConnectedState::handleCallResponse(common::PhoneNumber phoneNumber, bool pass){
    context.bts.sendCallResponse(phoneNumber, pass);
    if(pass){
        context.setState<TalkingState>(phoneNumber);
    }else{
        context.user.showMenuView();
    }
    context.timer.stopTimer();
}

void ConnectedState::handleSendCallRequest(common::PhoneNumber to){
    context.bts.sendCallRequest(to);
    using namespace std::chrono_literals;
    context.timer.startTimer(60000ms);
    context.user.showDialingView(to);
}

void ConnectedState::handleCallAccepted(common::PhoneNumber from){
    context.setState<TalkingState>(from);
    context.timer.stopTimer();
}

void ConnectedState::handleCallDrop(){
    context.timer.stopTimer();
    context.user.showMenuView();

}

}
