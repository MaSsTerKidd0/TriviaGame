#pragma once

#include "Communicator.h"
#include "IDatabase.h"

#define CLOSE_SERVER "EXIT"

class Server
{
public:
	Server(IDatabase* database);
	~Server();
	void run();

private:
	IDatabase* m_database;
	RequestHandlerFactory m_handlerFactory;
	Communicator m_communicator;
	void closeServer();
};

