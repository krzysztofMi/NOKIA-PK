#pragma once
#include <vector>
#include "Messages/PhoneNumber.hpp"
#include "Database/Entity/Sms.hpp"

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;
    virtual void handleSendMsg(common::PhoneNumber receiver, std::string content) = 0;
    virtual void handleGetSmsById(int id) = 0;
    virtual void handleGetAllSmsBySent(bool sent) = 0;
    virtual void handleUpdateSms(Sms sms) = 0;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;
    virtual void showReceivedSmsNotification() = 0;
    virtual void showSmsListView(std::vector<Sms>) = 0;
    virtual void showSmsView(Sms) = 0;
    virtual void showRequestCallView(common::PhoneNumber phoneNumber) = 0;
};

}
