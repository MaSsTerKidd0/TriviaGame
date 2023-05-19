#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "Server.h"
#include <iostream>
#include <exception>
#include "SqliteDatabase.h"

int main()
{
	try
	{
		WSAInitializer wsaInit;
		SqliteDatabase* dataAccess = new SqliteDatabase();
		Server myServer(dataAccess);
		myServer.run();
	}
	catch (std::exception& e)
	{
		std::cout << "Error occured: " << e.what() << std::endl;
	}
	system("PAUSE");
	return 0;
}