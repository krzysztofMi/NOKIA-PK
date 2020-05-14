#pragma once

#include "ISmsRepository.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Orm.hpp"

namespace ue {

using namespace sqlite_orm;
inline auto initStorage(const std::string &path){
    return make_storage(path,
                         make_table("sms_message",
                                    make_column("id", &Sms::id, autoincrement(), primary_key()),
                                    make_column("text", &Sms::text),
                                    make_column("phone_number", &Sms::phoneNumber),
                                    make_column("read", &Sms::read, default_value(false)),
                                    make_column("sent", &Sms::sent),
                                    make_column("failed", &Sms::failed, default_value(false))
                                    )
                         );
}

using SmsStorage = decltype(initStorage(""));

class SmsRepository: public ISmsRepository, public Orm<Sms,SmsStorage>
{
public:
    SmsRepository(common::ILogger& logger, std::string path);
    ~SmsRepository();

    std::vector<Sms> getAllBySent(bool sent) override;
};

}
