#include "Ports/SmsDatabasePort.hpp"

namespace ue {

    SmsDatabasePort::SmsDatabasePort(common::ILogger& logger, std::string databasePath): smsRepository{logger, databasePath}{
        sqlite3 *db;
        sqlite3_open(databasePath.c_str(), &db);
        const char* sql = "CREATE TABLE IF NOT EXISTS sms_message("
                            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
                            "text TEXT NOT NULL,"
                            "phone_number INTEGER NOT NULL,"
                            "read INTEGER NOT NULL DEFAULT 0,"
                            "sent INTEGER NOT NULL,"
                            "failed INTEGER NOT NULL DEFAULT 0"
                            ");";
        char *errMsg;
        int errorCode;
        errorCode = sqlite3_exec(db, sql, NULL, 0, &errMsg);
        if(errorCode){
            std::cout<<std::string(errMsg)<<std::endl;
        }
        sqlite3_close(db);
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
