#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


class RoomAdminRequestHandler : public IRequestHandler
{
public:
	RoomAdminRequestHandler(Room& room, LoggedUser user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory);
	~RoomAdminRequestHandler() = default;

	virtual bool isRequestRelevant(RequestInfo reqInfo);
	virtual RequestResult handleRequest(RequestInfo reqInfo);

private:
	Room& m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult closeRoom(RequestInfo reqInfo);
	RequestResult startGame(RequestInfo reqInfo);
	RequestResult getRoomState(RequestInfo reqInfo);
	RequestResult getPlayersInRoom(RequestInfo reqInfo);
	RequestResult Logout(RequestInfo reqInfo);
};