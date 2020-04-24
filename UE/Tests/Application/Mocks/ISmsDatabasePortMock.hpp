#pragma once
#include <gmock/gmock.h>
#include "Ports/ISmsDatabasePort.hpp"

namespace ue {

class ISmsDatabasePortMock : public ISmsDatabasePort{
public:
    ISmsDatabasePortMock();
    ~ISmsDatabasePortMock();

    MOCK_METHOD(Sms, saveSms, (std::string, common::PhoneNumber, bool, bool), (final));
    MOCK_METHOD(std::unique_ptr<Sms>, getSmsById, (int), (final));
    MOCK_METHOD(std::vector<Sms>, getAllSmsBySent, (bool), (final));
    MOCK_METHOD(void, updateSms, (Sms), (final));
};

}
