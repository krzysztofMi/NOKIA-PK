#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"
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
    setMenuCallbacks(menu);
}

void UserPort::showComposeSms(){
    IUeGui::ISmsComposeMode& composeSms = gui.setSmsComposeMode();

    //Callbacks
    gui.setAcceptCallback([&](){
        handler->handleSendMsg(composeSms.getPhoneNumber(),composeSms.getSmsText());
        composeSms.clearSmsText();
    });
    gui.setRejectCallback([&](){
        showMenuView();
    });
}

void UserPort::showSmsListView(std::vector<Sms> smsVector){
    IUeGui::IListViewMode& smsListView = gui.setListViewMode();
    ids.clear();
    smsListView.clearSelectionList();
    for(auto sms: smsVector){
        smsListView.addSelectionListItem(std::to_string(sms.phoneNumber), "");
        ids.push_back(sms.id);
    }
    //Callbacks
    gui.setAcceptCallback([&](){
        handler->handleGetSmsById(ids.at(smsListView.getCurrentItemIndex().second));
    });
    gui.setRejectCallback([&](){
        showMenuView();
    });
}

void UserPort::showSmsView(Sms sms){
    IUeGui::ITextMode& view = gui.setViewTextMode();
    view.setText(sms.text);

    //Callbacks
    gui.setAcceptCallback([&]{});
    gui.setRejectCallback([&]{
        switch(previousView){
            case View::SMS_RECEIVED:
                handler->handleGetAllSmsBySent(false);
                break;
            case View::SMS_SENT:
                handler->handleGetAllSmsBySent(true);
                break;
        }
    });
}

void UserPort::setMenuCallbacks(IUeGui::IListViewMode& menu){
    gui.setAcceptCallback([&](){
        switch(menu.getCurrentItemIndex().second){
            case 0:
                showComposeSms();
                break;
            case 1:
                previousView = View::SMS_RECEIVED;
                handler->handleGetAllSmsBySent(false);
                break;
            case 2:
                previousView = View::SMS_SENT;
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
    newCallView.setText("New call\n" + std::to_string(phoneNumber.value));
    gui.setAcceptCallback([&, phoneNumber]{
        handler->handleCallResponse(phoneNumber, true);
    });
    gui.setRejectCallback([&, phoneNumber]{
        handler->handleCallResponse(phoneNumber, false);
    });
}

void UserPort::showCallView(){
    IUeGui::ICallMode&  callView = gui.setCallMode();
}

}

