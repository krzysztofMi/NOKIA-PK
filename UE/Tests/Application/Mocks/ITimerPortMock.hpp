#pragma once

#include <gmock/gmock.h>
#include "Ports/ITimerPort.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class ITimerEventsHandlerMock : public ITimerEventsHandler
{
public:
    ITimerEventsHandlerMock();
    ~ITimerEventsHandlerMock() override;

    MOCK_METHOD(void, handleTimeout, (), (final));
    MOCK_METHOD(void, handleCallTimeout, (common::PhoneNumber), (final));

};

class ITimerPortMock : public ITimerPort
{
public:
    ITimerPortMock();
    ~ITimerPortMock() override;

    MOCK_METHOD(void, startTimer, (ITimerPort::Duration), (final));
    MOCK_METHOD(void, startCallTimer, (ITimerPort::Duration, common::PhoneNumber), (final));
    MOCK_METHOD(void, stopTimer, (), (final));
};

}
