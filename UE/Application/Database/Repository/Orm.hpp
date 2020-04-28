#pragma once
#include "include/sqlite_orm/sqlite_orm.h"
#include "Database/Entity/Sms.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Database/Repository/IOrm.hpp"

namespace ue {

template<class Entity, class Storage>
class Orm : public IOrm<Entity>{
public:
    Orm(common::ILogger& logger, std::string prefix, Storage storage) : logger{logger, prefix}
    , storage{storage}
    {}

    ~Orm() override{}
    std::unique_ptr<Entity> getById(int id) override{
       std::unique_ptr<Entity> entity = storage.template get_pointer<Entity>(id);
       if(entity == nullptr){
            logger.logError(Entity::getEntityName() + " with id: " + std::to_string(id) + " not found.");
            exit(0);
       }
       return entity;
    }

    std::vector<Entity> getAll() override{
        return storage.template get_all<Entity>();
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
        storage.template remove<Entity>(id);
        logger.logInfo(Entity::getEntityName() + " with id: " + std::to_string(id) + "removed from database.");
    }

protected:
    common::PrefixedLogger logger;
    Storage storage;
};

}
