#include "Server.h"
#include <exception>
#include <iostream>
#include <string>

#include "SqliteDatabase.h"


Server::Server(IDatabase* database) :
	m_handlerFactory(database), m_database(database), m_communicator(m_handlerFactory)
{
	// Thread that check when "EXIT" input will enter to the console to leave the server
	std::thread closeServerThread(&Server::closeServer, this);
	closeServerThread.detach();
}

Server::~Server()
{
	delete(m_database);
}

void Server::run()
{
	m_database->open();
	m_communicator.startHandleRequests();
}

/*
Function will get in to an endless loop till console get "EXIT" input
*/
void Server::closeServer()
{
	std::string serverInput = "";

	while (serverInput != CLOSE_SERVER)
	{
		std::cin >> serverInput;
	}
	std::cout << "Server Closed!" << std::endl;
	exit(0);
}
