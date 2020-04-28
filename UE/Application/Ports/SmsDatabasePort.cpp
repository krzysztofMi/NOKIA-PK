#include "Ports/SmsDatabasePort.hpp"

namespace ue {

    SmsDatabasePort::SmsDatabasePort(common::ILogger& logger, std::string databasePath): smsRepository{logger, databasePath}{
    }

    SmsDatabasePort::~SmsDatabasePort(){}

    void SmsDatabasePort::start(ISmsDatabaseEventsHandler handler){
       this->handler = &handler;
    }

    void SmsDatabasePort::stop(){
        handler = nullptr;
    }

    Sms SmsDatabasePort::saveSms(std::string text, common::PhoneNumber phoneNumber, bool read, bool sent){
        return smsRepository.insert(Sms{text, phoneNumber.value, read, sent});
    }

    std::unique_ptr<Sms> SmsDatabasePort::getSmsById(int id){
        return smsRepository.getById(id);
    }

    std::vector<Sms> SmsDatabasePort::getAllSmsBySent(bool sent){
        return smsRepository.getAllBySent(sent);
    }

    void SmsDatabasePort::updateSms(Sms sms){
        smsRepository.update(sms);
    }
}
