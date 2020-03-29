#include "SmsRepository.hpp"

namespace ue {

SmsRepository::SmsRepository(common::ILogger& logger)
    : logger(logger, "[SMS-DATABASE]"), smsDatabase{new SmsDatabase(logger)}{
}

SmsRepository::~SmsRepository(){}

void SmsRepository::save(Sms sms)
{
    std::string query = "INSERT INTO " + table + " (text, phone_number, read) VALUES ('" +
              sms.getText() + "', '" +
              std::to_string(sms.getPhoneNumber().value) + "', '" +
              std::to_string(sms.isRead()) + "');";
    smsDatabase->executeQuery(query, nullptr, NULL);
}

std::vector<Sms> SmsRepository::findAll()
{
    std::string query = "SELECT * FROM " + table;
    std::vector<Sms> result;
    try{
        smsDatabase->executeQuery(query, [](void* smsVector, int colNumber, char**rowFields, char**columnNames) -> int
        {
            auto vectorOfSms = static_cast<std::vector<Sms> *>(smsVector);
            Sms sms{};
            sms.setId(std::stol(rowFields[0]));
            sms.setText(rowFields[1]);
            sms.setPhoneNumber(common::PhoneNumber{static_cast<uint8_t>(atoi(rowFields[2]))});
            sms.setRead(rowFields[3] == "1" ? 1 : 0);
            vectorOfSms->push_back(sms);
            return 0;
        },
        static_cast<void*>(&result));
    }catch(SmsDatabase::QueryError error){
        result.clear();
        return result;
    }
    return result;
}

Sms SmsRepository::findById(long id)
{
    std::string query = "SELECT * FROM " + table + " WHERE id=" + std::to_string(id) +";";
    Sms sms;
    try{
        smsDatabase->executeQuery(query, [](void* input, int colNumber, char**rowFields, char**columnNames) -> int
        {
            std::cout<<rowFields[0]<<" "<<rowFields[1]<<" "<<rowFields[2]<<std::endl;
            auto sms = static_cast<Sms *>(input);
            sms->setId(std::stol(rowFields[0]));
            sms->setText(rowFields[1]);
            sms->setPhoneNumber(common::PhoneNumber{static_cast<uint8_t>(atoi(rowFields[2]))});
            sms->setRead(rowFields[3] == "1" ? 1 : 0);
            return 0;
        },static_cast<void*>(&sms));
    }catch(SmsDatabase::QueryError error){
        return Sms{};
    }
    return sms;
}

void SmsRepository::update(Sms sms)
{
    std::string query = "UPDATE " + table +
            " SET text = '" + sms.getText() +
            "', phone_number = '" + std::to_string(sms.getPhoneNumber().value) +
            "', read = '" + std::to_string(sms.isRead()) +
            "' WHERE id = " + std::to_string(sms.getId());
    logger.logInfo(query);
    try{
        smsDatabase->executeQuery(query, nullptr, nullptr);
    }catch(SmsDatabase::QueryError error){
        return;
    }
    logger.logInfo("Sms " + std::to_string(sms.getId()) + " updated.");
}

void SmsRepository::remove(Sms sms)
{
    std::string query = "DELETE FROM " + table + " WHERE id = " + std::to_string(sms.getId()) + ";";
    try{
        smsDatabase->executeQuery(query, nullptr, nullptr);
    }catch(SmsDatabase::QueryError error){
        return;
    }
    logger.logInfo("Sms id:" + std::to_string(sms.getId()) + " removed.");
}

}
