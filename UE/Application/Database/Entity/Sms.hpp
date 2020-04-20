#pragma once

#include <iostream>
#include "Messages/PhoneNumber.hpp"


namespace ue {

struct Sms{
  int id;
  std::string text;
  uint8_t phoneNumber;
  bool read; 
  bool sent; // 0 when sms was received, 1 when sms was sent

  static std::string getEntityName();
};

bool operator== (const Sms &s1, const Sms &s2)
{
    return (s1.id == s2.id && 
            s1.phoneNumber == s2.phoneNumber && 
            s1.text == s2.text &&
            s1.sent == s2.sent &&
            s1.read == s2.read );
}

}

