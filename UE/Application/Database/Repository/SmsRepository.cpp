#include "SmsRepository.hpp"

namespace ue {

SmsRepository::SmsRepository(
        common::ILogger& logger,
        std::string path
        ): Orm{logger, "[DATABASE-SMS]", initStorage(path)}{
}
SmsRepository::~SmsRepository(){}

std::vector<Sms> SmsRepository::getAllBySent(bool sent){
    return storage.get_all<Sms>(where(c(&Sms::sent) = (sent ? 1 : 0)));
}

}
