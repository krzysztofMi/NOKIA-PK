#pragma once

#include <iostream>
#include "Messages/PhoneNumber.hpp"


namespace ue {

struct Sms{
  int id;
  std::string text;
  uint8_t phoneNumber;
  bool read;
  bool sent;

  static std::string getEntityName();
};
}

