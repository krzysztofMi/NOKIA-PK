#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Database/Repository/IOrm.hpp"
#include "Database/Repository/SmsOrm.hpp"
namespace ue
{

class UserPort : public IUserPort
{
public:
    UserPort(common::ILogger& logger, IUeGui& gui, common::PhoneNumber phoneNumber, IOrm<Sms>& smsRepository);
    void start(IUserEventsHandler& handler);
    void stop();

    void showNotConnected() override;
    void showConnecting() override;
    void showConnected() override;
    void showComposeSms();
    void showSmsListView();
    void showSmsListViewSent();
    void smsView(int id);

private:
    IOrm<Sms>& smsRepository;
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    IUserEventsHandler* handler = nullptr;

    void setMenuCallbacks(IUeGui::IListViewMode& menu);
};

}
