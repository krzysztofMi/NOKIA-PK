#pragma once

#include "IEventsHandler.hpp"
#include "Logger/ILogger.hpp"
#include "Database/Repository/SmsOrm.hpp"

namespace ue
{

struct Context
{
    common::ILogger& logger;
    IBtsPort& bts;
    IUserPort& user;
    ITimerPort& timer;
    IOrm<Sms>& sms;
    std::unique_ptr<IEventsHandler> state{};
    template <typename State, typename ...Arg>
    void setState(Arg&& ...arg)
    {
        state = std::make_unique<State>(*this, std::forward<Arg>(arg)...);
    }
};

}
