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

bool operator== (const Sms &s1, const Sms &s2);

}

