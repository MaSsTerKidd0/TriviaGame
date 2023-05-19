#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

class RoomMemberRequestHandler : public IRequestHandler
{
public:
	RoomMemberRequestHandler(Room& room, LoggedUser user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory);
	~RoomMemberRequestHandler() = default;

	virtual bool isRequestRelevant(RequestInfo reqInfo);
	virtual RequestResult handleRequest(RequestInfo reqInfo);

private:
	Room& m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult leaveRoom(RequestInfo reqInfo);
	RequestResult getRoomState(RequestInfo reqInfo);
	RequestResult getPlayersInRoom(RequestInfo reqInfo);
	RequestResult Logout(RequestInfo reqInfo);

};
