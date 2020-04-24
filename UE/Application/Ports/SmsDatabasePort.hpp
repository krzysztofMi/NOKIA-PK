#pragma once
#include "Logger/PrefixedLogger.hpp"
#include "Ports/ISmsDatabasePort.hpp"

namespace ue {

class SmsDatabasePort: public ISmsDatabasePort{
public:
    SmsDatabasePort(common::ILogger& logger, std::string databasePath);
    ~SmsDatabasePort();
    Sms saveSms(std::string text, common::PhoneNumber phoneNumber, bool read, bool sent) override;
    std::unique_ptr<Sms> getSmsById(int id) override;
    std::vector<Sms> getAllSmsBySent(bool sent) override;
    void updateSms(Sms sms) override;
    void start(ISmsDatabaseEventsHandler);
    void stop();
private:
    SmsRepository smsRepository;
    ISmsDatabaseEventsHandler* handler = nullptr;
};

}

