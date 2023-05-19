#pragma once
#include "IRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestHandlerFactory.h"
#include "MenuRequestHandler.h"
#include "LoginManager.h"
#include "Codes.h"

class RequestHandlerFactory;
class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory& handlerFactory);
	~LoginRequestHandler() = default;

	virtual bool isRequestRelevant(RequestInfo reqInfo);
	virtual RequestResult handleRequest(RequestInfo reqInfo);
private:

	RequestHandlerFactory& m_handlerFactory;
	LoginManager& m_loginManager;
	RequestResult login(RequestInfo reqInfo);
	RequestResult signup(RequestInfo reqInfo);
};