#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"

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
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");

    gui.setAcceptCallback([&](){
        std::cout<<menu.getCurrentItemIndex().first<<std::endl;
        switch(menu.getCurrentItemIndex().second){
            case 0:
                showComposeSms();
                break;
            case 1:
                menu.clearSelectionList();
                showSmsListView();
                break;
        }
    });
}

void UserPort::showComposeSms(){
    IUeGui::ISmsComposeMode& composeSms = gui.setSmsComposeMode();
    //ADD REJECT CALLBACK (RETURN TO MENU).
}

void UserPort::showSmsListView(){
    IUeGui::IListViewMode& smsListView = gui.setListViewMode();
    //TODO ADD SMS TO LIST WITH READ/UNREAD FIELD
    //ADD MOVE TO smsVIEW
    //ADD REJECT CALLBACK(RETURN TO MENU)
}

}
