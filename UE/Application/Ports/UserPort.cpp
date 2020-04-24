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

void UserPort::start(IUserEventsHandler &handler)
{
    this->handler = &handler;
    gui.setTitle("Nokia " + to_string(phoneNumber));
}

void UserPort::stop()
{
    handler = nullptr;
}

void UserPort::showNotConnected()
{
    gui.showNotConnected();
}

void UserPort::showConnecting()
{
    gui.showConnecting();
}

void UserPort::showConnected()
{
    gui.showConnected();
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("SMS Received", "");
    menu.addSelectionListItem("SMS Sent", "");
    setMenuCallbacks(menu);
}

void UserPort::showComposeSms(){
    IUeGui::ISmsComposeMode& composeSms = gui.setSmsComposeMode();
    gui.setAcceptCallback([&](){
        handler->handleSendMsg(composeSms.getPhoneNumber(),composeSms.getSmsText());
        composeSms.clearSmsText();
    });
    gui.setRejectCallback([&](){
        showConnected();
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
    gui.setAcceptCallback([&](){
        handler->handleGetSmsById(ids.at(smsListView.getCurrentItemIndex().second));
    });
    gui.setRejectCallback([&](){
        showConnected();
    });
}

void UserPort::showSmsView(Sms sms){
    IUeGui::ITextMode& view = gui.setViewTextMode();
    view.setText(sms.text);
}

void UserPort::setMenuCallbacks(IUeGui::IListViewMode& menu){
    gui.setAcceptCallback([&](){
        switch(menu.getCurrentItemIndex().second){
            case 0:
                showComposeSms();
                break;
            case 1:
                handler->handleGetAllSmsBySent(false);
                break;
            case 2:
                handler->handleGetAllSmsBySent(true);
                break;
        }
    });
}

void UserPort::showReceivedSmsNotification()
{
    gui.showNewSms();
}
}

