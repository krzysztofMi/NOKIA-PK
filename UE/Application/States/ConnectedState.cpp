#include "ConnectedState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
}

    void ConnectedState::handleSmsReceived(common::PhoneNumber phoneNumber, 
                                           std::string msg)
    {
        Sms incomingSms;
        incomingSms.phoneNumber = phoneNumber.value;
        incomingSms.text = msg;
        incomingSms.read = false;
        incomingSms.sent = false;

        context.sms.insert(incomingSms);   
    }

}
