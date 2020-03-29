#pragma once

#include <memory>
#include "Database/SmsDatabase.hpp"
#include "ICrudRepository.hpp"
#include "Logger/PrefixedLogger.hpp"

namespace ue {

class SmsRepository : public ICrudSmsRepository{
public:
    SmsRepository(common::ILogger& logger);
    ~SmsRepository();
    void save(Sms sms);
    std::vector<Sms> findAll();
    Sms findById(long);
    void update(Sms sms);
    void remove(Sms sms);
private:
    common::PrefixedLogger logger;
    std::unique_ptr<IDatabase> smsDatabase;
    const std::string table = "sms_message";
};

}
