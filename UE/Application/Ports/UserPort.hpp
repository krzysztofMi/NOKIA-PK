#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Database/Repository/IOrm.hpp"
#include "Database/Repository/Orm.hpp"
namespace ue
{

enum class View{MENU, COMPOSE_SMS, SMS_SENT, SMS_RECEIVED, SMS, CALL};

class UserPort : public IUserPort
{
public:
    UserPort(common::ILogger& logger,
            IUeGui& gui,
            common::PhoneNumber phoneNumber);

    void start(IUserEventsHandler& handler);
    void stop();

    void showNotConnected() override;
    void showPeerUeBecomesUnknown(common::PhoneNumber phoneNumber) override;
    void showConnecting() override;
    void showConnected() override;
    void showReceivedSmsNotification() override;
    void showMenuView() override;
    void showComposeSms();
    void clearCallMessages() override;
    void showSmsListView(std::vector<Sms> smsVector) override;
    void showSmsView(Sms sms) override;
    void showRequestCallView(common::PhoneNumber phoneNumber) override;
    void showStartDialView() override;
    void showDialingView(common::PhoneNumber to) override;
    void showCallView(const std::string incomingMessage) override;

private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    common::PhoneNumber receiverNumber;
    IUserEventsHandler* handler = nullptr;
    std::vector<int> ids;
    View view;

    void setMenuCallbacks(IUeGui::IListViewMode& menu);
    void setDummyAcceptCallback();
    void returnToMenuCallback();
};

}
