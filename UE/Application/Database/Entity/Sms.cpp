#include "Sms.hpp"


namespace ue {

Sms::Sms(){}
Sms::~Sms(){}

Sms::Sms(std::string text, common::PhoneNumber phoneNumber)
    : text(text),
      phoneNumber(phoneNumber){}

Sms::Sms(std::string text, common::PhoneNumber phoneNumber, bool read)
    : text(text),
      phoneNumber(phoneNumber),
      read(read){}

Sms::Sms(long id, std::string text, common::PhoneNumber phoneNumber)
    : id(id),
      text(text),
      phoneNumber(phoneNumber){}

Sms::Sms(long id, std::string text, common::PhoneNumber phoneNumber, bool read)
    : id(id),
      text(text),
      phoneNumber(phoneNumber),
      read(read){}

long Sms::getId(){return id;}
common::PhoneNumber Sms::getPhoneNumber(){return phoneNumber;}
std::string Sms::getText(){return text;}
bool Sms::isRead(){return read;}

void Sms::setId(long id){this->id = id;}
void Sms::setPhoneNumber(common::PhoneNumber phoneNumber){this->phoneNumber = phoneNumber;}
void Sms::setText(std::string text){this->text = text;}
void Sms::setRead(bool isRead){this->read = isRead;}
}
