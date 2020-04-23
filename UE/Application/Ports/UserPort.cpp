#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include <vector>
#include <iostream>
#include <string>


namespace ue
{

UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber, IOrm<Sms>& smsRepository)
    : logger(logger, "[USER-PORT]"),
      gui(gui),
      phoneNumber(phoneNumber),
      smsRepository{smsRepository}

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

void UserPort::smsView(int id){
    IUeGui::ITextMode& view = gui.setViewTextMode();
    std::unique_ptr<Sms> sms = smsRepository.get(id);
    view.setText(sms->text);

}

void UserPort::showSmsListView(){
    IUeGui::IListViewMode& smsListView = gui.setListViewMode();


    std::vector<Sms> tablica = smsRepository.getAll();
    int smsFromDatabase;
    for (int i = 0; i < tablica.size(); i++) {
        if(tablica.at(i).sent==true){
            smsFromDatabase=tablica.at(i).phoneNumber;
            std::string str= std::to_string(smsFromDatabase);
            smsListView.addSelectionListItem(str, "");
            gui.setAcceptCallback([&](){
                smsView(smsListView.getCurrentItemIndex().second+1);

                });
            gui.setRejectCallback([&](){
                showConnected();
            });
        }
    }
}

void UserPort::showSmsListViewSent(){
    IUeGui::IListViewMode& smsListView = gui.setListViewMode();


    std::vector<Sms> tablica = smsRepository.getAll();
    int smsFromDatabase;
    for (int i = 0; i < tablica.size(); i++) {
        if(tablica.at(i).sent==false){
            smsFromDatabase=tablica.at(i).phoneNumber;
            std::string str= std::to_string(smsFromDatabase);
            smsListView.addSelectionListItem(str, "");
            gui.setAcceptCallback([&](){
                smsView(smsListView.getCurrentItemIndex().second+1);

                });
            gui.setRejectCallback([&](){
                showConnected();
            });
        }
    }
}


void UserPort::setMenuCallbacks(IUeGui::IListViewMode& menu){
    gui.setAcceptCallback([&](){
        switch(menu.getCurrentItemIndex().second){
            case 0:
                showComposeSms();
                break;
            case 1:
                menu.clearSelectionList();
                showSmsListView();
                break;
            case 2:
                menu.clearSelectionList();
                showSmsListViewSent();
                break;
        }
    });
}

void UserPort::showReceivedSmsNotification()
{
    gui.showNewSms();
}

}

