#pragma once
#include "Database/Repository/SmsRepository.hpp"

namespace ue {

class ISmsDatabaseEventsHandler{
public:
    virtual ~ISmsDatabaseEventsHandler() = default;
};

class ISmsDatabasePort{
public:
    virtual ~ISmsDatabasePort() = default;
    virtual Sms saveSms(std::string text,
                        common::PhoneNumber phoneNumber,
                        bool read,
                        bool sent) = 0;
    virtual std::unique_ptr<Sms> getSmsById(int id) = 0;
    virtual std::vector<Sms> getAllSmsBySent(bool sent) = 0;
    virtual void updateSms(Sms sms) = 0;
};

}
