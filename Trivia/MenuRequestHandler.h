#pragma once
#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"
#include "Codes.h"

class StatisticsManager;
class RequestHandlerFactory;
class MenuRequestHandler : public IRequestHandler
{
public:
	MenuRequestHandler(RequestHandlerFactory& handlerFactory, RoomManager& roomManager, StatisticsManager& statisticsManager, const std::string username);
	~MenuRequestHandler() = default;
 
	virtual bool isRequestRelevant(RequestInfo reqInfo);
	virtual RequestResult handleRequest(RequestInfo reqInfo);

private:
	RequestHandlerFactory& m_handlerFactory;
	RoomManager& m_roomManager;
	StatisticsManager& m_statisticsManager;
	LoggedUser m_user;

	RequestResult signout(RequestInfo reqInfo);
	RequestResult getRooms(RequestInfo reqInfo);
	RequestResult getPlayersInRoom(RequestInfo reqInfo);
	RequestResult getPersonalStats(RequestInfo reqInfo);
	RequestResult getHighScore(RequestInfo reqInfo);
	RequestResult joinRoom(RequestInfo reqInfo);
	RequestResult createRoom(RequestInfo reqInfo);
	RequestResult getRoomSettings(RequestInfo reqInfo);
};