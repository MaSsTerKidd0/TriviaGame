#pragma once
#include "LoginRequestHandler.h"
#include "LoginManager.h"
#include "IDatabase.h"
#include "MenuRequestHandler.h"
#include "StatisticsManager.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

class RoomMemberRequestHandler;
class RoomAdminRequestHandler;
class StatisticsManager;
class LoginRequestHandler;
class MenuRequestHandler;
class RequestHandlerFactory
{
public:
	RequestHandlerFactory(IDatabase* dataAccess);
	
	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler(const std::string username);
	RoomAdminRequestHandler* createRoomAdminRequestHandler(Room& room, LoggedUser user);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(Room& room, LoggedUser user);

	LoginManager& getLoginManager();
	StatisticsManager& getStatisticsManager();
	RoomManager& getRoomManager();


private:
	IDatabase* m_database;
	LoginManager m_loginManager;
	RoomManager m_roomManager;
	StatisticsManager m_statisticsManager;
};
