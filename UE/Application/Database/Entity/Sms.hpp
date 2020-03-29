#pragma once

#include <iostream>
#include "Messages/PhoneNumber.hpp"

namespace ue {

class Sms {
    long id;
    std::string text;
    common::PhoneNumber phoneNumber;
    bool read;
public:
    Sms();
    Sms(std::string text, common::PhoneNumber phoneNumber);
    Sms(std::string text, common::PhoneNumber phoneNumber, bool read);
    Sms(long id, std::string text, common::PhoneNumber phoneNumber);
    Sms(long id, std::string text, common::PhoneNumber phoneNumber, bool read);
    ~Sms();

    long getId();
    void setId(long id);

    std::string getText();
    void setText(std::string text);

    common::PhoneNumber getPhoneNumber();
    void setPhoneNumber(common::PhoneNumber phoneNumber);

    bool isRead();
    void setRead(bool isRead);
};
}
