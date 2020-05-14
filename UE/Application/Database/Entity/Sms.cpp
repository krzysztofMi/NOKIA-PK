#include "Sms.hpp"


namespace ue {

std::string Sms::getEntityName(){
    return "Sms";
}

Sms::Sms(std::string text, uint8_t phoneNumber, bool read, bool sent, bool failed) :
    text{text},
    phoneNumber{phoneNumber},
    read{read},
    sent{sent},
    failed{failed} {}

bool operator== (const Sms &s1, const Sms &s2)
{
    return (s1.id == s2.id && 
            s1.phoneNumber == s2.phoneNumber && 
            s1.text == s2.text &&
            s1.sent == s2.sent &&
            s1.read == s2.read );
}

}
