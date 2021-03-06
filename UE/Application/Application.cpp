#include "Application.hpp"
#include "States/NotConnectedState.hpp"

namespace ue
{

Application::Application(common::PhoneNumber phoneNumber,
                         common::ILogger &iLogger,
                         IBtsPort &bts,
                         IUserPort &user,
                         ITimerPort &timer,
                         ISmsDatabasePort &database)
    : context{iLogger, bts, user, timer, database},
      logger(iLogger, "[APP] ")
{
    logger.logInfo("Started");
    context.setState<NotConnectedState>();
}

Application::~Application()
{
    logger.logInfo("Stopped");
}

void Application::handleTimeout()
{
    context.state->handleTimeout();
}

void Application::handleDisconnected()
{
    context.state->handleDisconnected();
}

void Application::handleSib(common::BtsId btsId)
{
    context.state->handleSib(btsId);
}

void Application::handleAttachAccept()
{
    context.state->handleAttachAccept();
}

void Application::handleAttachReject()
{
    context.state->handleAttachReject();
}

void Application::handleSmsReceived(common::PhoneNumber from, std::string message)
{
    context.state->handleSmsReceived(from, message);
}

void Application::handleFailedToSendSms()
{
    context.state->handleFailedToSendSms();
}

void Application::handlePeerUeBecomesUnknown(){
    context.state->handlePeerUeBecomesUnknown();
}

void Application::handleSendMsg(common::PhoneNumber receiver, std::string content) {
    context.state->handleSendMsg(receiver, content);
}

void Application::handleGetAllSmsBySent(bool sent){
    context.state->handleGetAllSmsBySent(sent);
}

void Application::handleGetSmsById(int id){
    context.state->handleGetSmsById(id);
}

void Application::handleUpdateSms(Sms sms){
    context.state->handleUpdateSms(sms);
}

void Application::handleCallRequest(common::PhoneNumber phoneNumber){
    context.state->handleCallRequest(phoneNumber);
}

void Application::handleCallResponse(common::PhoneNumber phoneNumber, bool pass){
    context.state->handleCallResponse(phoneNumber, pass);
}

void Application::handleCallAccepted(common::PhoneNumber phoneNumber){
    context.state->handleCallAccepted(phoneNumber);
}

void Application::handleSendCallRequest(common::PhoneNumber to){
    context.state->handleSendCallRequest(to);
}

void Application::handleSendTalkMessage(const std::string incomingMessage){
    context.state->handleSendTalkMessage(incomingMessage);
}

void Application::handleTalkMessage(const std::string message){
    context.state->handleTalkMessage(message);
}

void Application::handleCallDrop(){
    context.state->handleCallDrop();
}

void Application::handleSendCallDrop(common::PhoneNumber sender, common::PhoneNumber receiver)
{
    context.state->handleSendCallDrop(sender,receiver);
}

}
