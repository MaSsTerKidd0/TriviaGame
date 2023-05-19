#pragma once

#include "LoginRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <map>
#include <thread>

#define PORT 1231	
#define VALID_CHAR_START 31
#define VALID_CHAR_END 126

class Communicator
{
public:
	Communicator(RequestHandlerFactory& handlerFactory);
	~Communicator();

	void startHandleRequests();
	void buildRequestInfoStruct(RequestInfo& reqInfo, SOCKET socket);

private:

	void bindAndListen();
	void handleNewClient(SOCKET socket);
	void accept();
	std::string buildResponeBuffer(RequestResult result, int responeCode);
	int getResponeCode(int requestId);

	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory& m_handlerFactory;
};