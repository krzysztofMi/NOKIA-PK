#pragma once
#include <vector>
#include "Database/Entity/Sms.hpp"

namespace ue {

class ISmsRepository{
public:
    virtual ~ISmsRepository() = default;
    virtual std::vector<Sms> getAllBySent(bool sent) = 0;
};

}
