#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Database/Repository/IOrm.hpp"
#include "Database/Repository/Orm.hpp"
namespace ue
{

class UserPort : public IUserPort
{
public:
    UserPort(common::ILogger& logger,
            IUeGui& gui,
            common::PhoneNumber phoneNumber);

    void start(IUserEventsHandler& handler);
    void stop();

    void showNotConnected() override;
    void showConnecting() override;
    void showConnected() override;
    void showReceivedSmsNotification() override;
    void showComposeSms();
    void showSmsListView(std::vector<Sms> smsVector) override;
    void showSmsView(Sms sms) override;

private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    IUserEventsHandler* handler = nullptr;
    std::vector<int> ids;
    void setMenuCallbacks(IUeGui::IListViewMode& menu);
};

}
