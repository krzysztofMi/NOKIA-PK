#pragma once

#include <vector>
#include "Database/Entity/Sms.hpp"

namespace ue {

class ICrudSmsRepository{
public:
    virtual ~ICrudSmsRepository() = default;
    virtual void save(Sms) = 0;
    virtual std::vector<Sms> findAll() = 0;
    virtual Sms findById(long) = 0;
    virtual void update(Sms) = 0;
    virtual void remove(Sms) = 0;
};

}
