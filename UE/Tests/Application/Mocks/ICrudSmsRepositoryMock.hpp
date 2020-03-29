#pragma once

#include <gmock/gmock.h>
#include "Database/Repository/ICrudRepository.hpp"
#include "Database/IDatabase.hpp"
#include "Database/Entity/Sms.hpp"

namespace ue {

class ICrudSmsRepositoryMock : public ICrudSmsRepository
{
public:
    ICrudSmsRepositoryMock();
   ~ICrudSmsRepositoryMock() override;

    MOCK_METHOD(void, save, (Sms), (final));
    MOCK_METHOD(std::vector<Sms>, findAll, (), (final));
    MOCK_METHOD(Sms, findById, (long), (final));
    MOCK_METHOD(void, update, (Sms), (final));
    MOCK_METHOD(void, remove, (Sms), (final));
};
}
