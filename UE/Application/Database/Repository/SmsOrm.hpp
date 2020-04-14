#pragma once
#include "include/sqlite_orm/sqlite_orm.h"
#include "Database/Entity/Sms.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Database/Repository/IOrm.hpp"

namespace ue {
using namespace sqlite_orm;

inline auto initStorage(const std::string &path){
    return make_storage(path,
                         make_table("sms_message",
                                    make_column("id", &Sms::id, autoincrement(), primary_key()),
                                    make_column("text", &Sms::text),
                                    make_column("phone_number", &Sms::phoneNumber),
                                    make_column("read", &Sms::read, default_value(false)),
                                    make_column("sent", &Sms::sent)
                                    )
                         );
}

using SmsStorage = decltype(initStorage(""));

template<class Entity>
class Orm : public IOrm<Entity>{
public:
    Orm(common::ILogger& logger, std::string prefix) : logger{logger, prefix},
        storage{initStorage("../../NOKIA-PK/UE/Application/Database/Schema/smsdatabase.db")}{
    }
    ~Orm() override{}
    std::unique_ptr<Entity> get(int id) override{
       std::unique_ptr<Entity> entity = storage.get_pointer<Entity>(id);
       if(entity == nullptr){
            logger.logError(Entity::getEntityName() + " with id: " + std::to_string(id) + " not found.");
       }
       return entity;
    }

    std::vector<Entity> getAll() override{
        return storage.get_all<Entity>();
    }

    Entity insert(Entity elem) override{
        auto id = storage.insert(elem);
        logger.logInfo(Entity::getEntityName() + " with id: " + std::to_string(id) + " created.");
        elem.id = id;
        return elem;
    }
    void update(Entity elem) override{
        storage.update(elem);
        logger.logInfo(Entity::getEntityName() + " with id: " + std::to_string(elem.id) + " updated.");
    }
    void remove(int id) override{
        storage.remove<Entity>(id);
        logger.logInfo(Entity::getEntityName() + " with id: " + std::to_string(id) + "removed from database.");
    }
private:
    common::PrefixedLogger logger;
    SmsStorage storage;
};

}
