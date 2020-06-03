#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include "UeGui/IDialMode.hpp"
#include "UeGui/ICallMode.hpp"
#include <vector>
#include <iostream>
#include <string>

namespace ue
{

UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber)
    : logger(logger, "[USER-PORT]"),
      gui(gui),
      phoneNumber(phoneNumber)
{}

void UserPort::start(IUserEventsHandler &handler){
    this->handler = &handler;
    gui.setTitle("Nokia " + to_string(phoneNumber));
}

void UserPort::stop(){
    handler = nullptr;
}

void UserPort::showNotConnected(){
    gui.showNotConnected();
}

void UserPort::showPeerUeBecomesUnknown(common::PhoneNumber phoneNumber){
    gui.showPeerUserNotAvailable(phoneNumber);
}

void UserPort::showConnecting(){
    gui.showConnecting();
}

void UserPort::showConnected(){
    gui.showConnected();
}

void UserPort::showMenuView(){
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("SMS Received", "");
    menu.addSelectionListItem("SMS Sent", "");
    menu.addSelectionListItem("Call", "");
    setMenuCallbacks(menu);
}

void UserPort::showComposeSms(){
    IUeGui::ISmsComposeMode& composeSms = gui.setSmsComposeMode();

    //Callbacks
    gui.setAcceptCallback([&](){
        handler->handleSendMsg(composeSms.getPhoneNumber(),composeSms.getSmsText());
        composeSms.clearSmsText();
    });
    returnToMenuCallback();
}

void UserPort::returnToMenuCallback(){
    gui.setRejectCallback([&](){
        showMenuView();
    });
}

void UserPort::showSmsListView(std::vector<Sms> smsVector){
    IUeGui::IListViewMode& smsListView = gui.setListViewMode();
    ids.clear();
    smsListView.clearSelectionList();
    for(auto sms: smsVector){
        if (sms.read == true){
            if(sms.failed) smsListView.addSelectionListItem(std::to_string(sms.phoneNumber)+" Failed", "");
            else smsListView.addSelectionListItem(std::to_string(sms.phoneNumber), "");
        }
        else{
            std::string smsStatus = std::to_string(sms.phoneNumber);
            smsStatus=smsStatus+" Unread";
            smsListView.addSelectionListItem(smsStatus,"");

        }

        ids.push_back(sms.id);
    }
    //Callbacks
    gui.setAcceptCallback([&](){
        handler->handleGetSmsById(ids.at(smsListView.getCurrentItemIndex().second));
    });
    returnToMenuCallback();
}

void UserPort::showSmsView(Sms sms){
    IUeGui::ITextMode& smsView = gui.setViewTextMode();
    smsView.setText(sms.text);
    if(sms.read != true){
        sms.read = true;
        handler->handleUpdateSms(sms);
    }
    //Callbacks
    gui.setAcceptCallback(nullptr);
    gui.setRejectCallback([&]{
        switch(view){
            case View::SMS_RECEIVED:
                handler->handleGetAllSmsBySent(false);
                break;
            case View::SMS_SENT:
                handler->handleGetAllSmsBySent(true);
                break;
        }
    });
}

void UserPort::showReceivedSmsNotification()
{
    gui.showNewSms();
}

void UserPort::showRequestCallView(common::PhoneNumber phoneNumber){
    IUeGui::ITextMode& newCallView = gui.setAlertMode();
    newCallView.setText("New call from\n" + std::to_string(phoneNumber.value));
    gui.setAcceptCallback([&, phoneNumber]{
        receiverNumber = phoneNumber;
        handler->handleCallResponse(phoneNumber, true);
    });
    gui.setRejectCallback([&, phoneNumber]{
        receiverNumber.value = 0;
        handler->handleCallResponse(phoneNumber, false);
    });
}

void UserPort::showStartDialView(){
    IUeGui::IDialMode&  callView = gui.setDialMode();
    gui.setAcceptCallback([&](){
        if(callView.getPhoneNumber().value!=phoneNumber.value)
            handler->handleSendCallRequest(callView.getPhoneNumber());
    });
    returnToMenuCallback();
}

void UserPort::showDialingView(common::PhoneNumber to){
    IUeGui::ITextMode& dialingView = gui.setAlertMode();
    receiverNumber = to;
    dialingView.setText("Waiting for call accept...");
    gui.setAcceptCallback(nullptr);
    gui.setRejectCallback([&, to]{
        receiverNumber.value = 0;
        handler->handleCallResponse(to, false);
    });
}

void UserPort::showCallView(const std::string incomingText){
    IUeGui::ICallMode& callView = gui.setCallMode();
    callView.appendIncomingText(incomingText);
    gui.setAcceptCallback([&](){
        handler->handleSendTalkMessage(callView.getOutgoingText());
        callView.clearOutgoingText();

    });

    gui.setRejectCallback([&](){
        if(receiverNumber.value!=0)
            handler->handleSendCallDrop(phoneNumber,receiverNumber);
        else showMenuView();
    });
}

void UserPort::clearCallMessages() {
    IUeGui::ICallMode& callView = gui.setCallMode();
    callView.clearIncomingText();
}

void UserPort::setMenuCallbacks(IUeGui::IListViewMode& menu){
    gui.setAcceptCallback([&](){
        switch(menu.getCurrentItemIndex().second){
            case 0:
                view = View::COMPOSE_SMS;
                showComposeSms();
                break;
            case 1:
                view = View::SMS_RECEIVED;
                handler->handleGetAllSmsBySent(false);
                break;
            case 2:
                view = View::SMS_SENT;
                handler->handleGetAllSmsBySent(true);
                break;
            case 3:
                view = View::CALL;
                showStartDialView();
        }
    });
}

}
