#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* database) :
	m_database(database), m_loginManager(database), m_statisticsManager(database)
{
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	LoginRequestHandler* loginRequest = new LoginRequestHandler(*this);
	return loginRequest;
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return m_loginManager;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const std::string username)
{
	MenuRequestHandler* menuRequestHandler = new MenuRequestHandler(*this, m_roomManager, m_statisticsManager, username);
	return menuRequestHandler;
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(Room& room, LoggedUser user)
{
	RoomAdminRequestHandler* roomAdminRequestHandler = new RoomAdminRequestHandler(room, user, m_roomManager, *this);
	return roomAdminRequestHandler;
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(Room& room, LoggedUser user)
{
	RoomMemberRequestHandler* roomMemberRequestHandler = new RoomMemberRequestHandler(room, user, m_roomManager, *this);
	return roomMemberRequestHandler;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
	return this->m_statisticsManager;
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
	return this->m_roomManager;
}
