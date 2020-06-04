#pragma once

#include "IBtsPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "ITransport.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class BtsPort : public IBtsPort
{
public:
    BtsPort(common::ILogger& logger, common::ITransport& transport, common::PhoneNumber phoneNumber);
    void start(IBtsEventsHandler& handler);
    void stop();

    void sendAttachRequest(common::BtsId) override;
    void sendMsg(common::PhoneNumber receiver, std::string content) override;

    void sendCallResponse(common::PhoneNumber, bool) override;
    void sendCallRequest(common::PhoneNumber to) override;
    void sendTalkMessage(const std::string, const common::PhoneNumber) override;
    void sendCallDrop(common::PhoneNumber sender, common::PhoneNumber receiver) override;
private:
    void handleMessage(BinaryMessage msg);
    void handleDisconnected();
    void handleCallDrop();
    common::PrefixedLogger logger;
    common::ITransport& transport;
    common::PhoneNumber phoneNumber;

    IBtsEventsHandler* handler = nullptr;
};

}
