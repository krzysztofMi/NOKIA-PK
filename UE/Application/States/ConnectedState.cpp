#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
}


void ConnectedState::handleSmsReceived(common::PhoneNumber phoneNumber, 
                                       std::string msg)
{
<<<<<<< Updated upstream
    Sms incomingSms;
    incomingSms.phoneNumber = phoneNumber.value;
    incomingSms.text = msg;
    incomingSms.read = false;
    incomingSms.sent = false;

    context.sms.insert(incomingSms);   
    context.user.showReceivedSmsNotification();
=======
    context.database.saveSms(msg, phoneNumber, false, false);
>>>>>>> Stashed changes
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

}
