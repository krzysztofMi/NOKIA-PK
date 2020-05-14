#pragma once

#include <iostream>
#include "Messages/PhoneNumber.hpp"


namespace ue {

struct Sms{

    Sms(){}
    Sms(std::string text, uint8_t phoneNumber, bool read, bool sent, bool failed = false);

    int id;
    std::string text;
    uint8_t phoneNumber;
    bool read;
    bool sent; // 0 when sms was received, 1 when sms was sent
    bool failed;
    static std::string getEntityName();
};

bool operator== (const Sms &s1, const Sms &s2);

}

