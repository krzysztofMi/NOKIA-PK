#pragma once

#include <gmock/gmock.h>
#include "Ports/IUserPort.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Database/Entity/Sms.hpp"

namespace ue
{

class IUserEventsHandlerMock : public IUserEventsHandler
{
public:
    IUserEventsHandlerMock();
    ~IUserEventsHandlerMock() override;
    MOCK_METHOD(void, handleSendMsg, (common::PhoneNumber,std::string), (final));
    MOCK_METHOD(void, handleGetAllSmsBySent, (bool), (final));
    MOCK_METHOD(void, handleGetSmsById, (int), (final));
    MOCK_METHOD(void, handleUpdateSms, (Sms), (final));
    MOCK_METHOD(void, handleCallResponse, (common::PhoneNumber, bool), (final));
};

class IUserPortMock : public IUserPort
{
public:
    IUserPortMock();
    ~IUserPortMock() override;

    MOCK_METHOD(void, showNotConnected, (), (final));
    MOCK_METHOD(void, showConnecting, (), (final));
    MOCK_METHOD(void, showConnected, (), (final));
    MOCK_METHOD(void, showMenuView, (), (final));
    MOCK_METHOD(void, showReceivedSmsNotification, (), (final));
    MOCK_METHOD(void, showSmsListView, (std::vector<Sms>), (final));
    MOCK_METHOD(void, showSmsView, (Sms), (final));
    MOCK_METHOD(void, showRequestCallView, (common::PhoneNumber), (final));
    MOCK_METHOD(void, showCallView, (), (final));
};

}
