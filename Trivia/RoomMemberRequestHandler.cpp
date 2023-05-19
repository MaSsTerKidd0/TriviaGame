#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(Room& room, LoggedUser user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory) : 
	m_room(room), m_user(user), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo reqInfo)
{
    return (reqInfo.id == LEAVE_ROOM_REQUEST || reqInfo.id == GET_PLAYERS_IN_ROOM_REQUEST || reqInfo.id == GET_ROOM_STATE_REQUEST || reqInfo.id == LOGOUT_REQUEST);
}

RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo reqInfo)
{
    RequestResult reqResult;
    ErrorRespone errorRes;
    if (isRequestRelevant(reqInfo))
    {
        switch (reqInfo.id)
        {
        case LEAVE_ROOM_REQUEST:
            reqResult = leaveRoom(reqInfo);
            break;
        case GET_ROOM_STATE_REQUEST:
            reqResult = getRoomState(reqInfo);
            break;
        case GET_PLAYERS_IN_ROOM_REQUEST:
            reqResult = getPlayersInRoom(reqInfo);
            break;
        case LOGOUT_REQUEST:
            reqResult = Logout(reqInfo);
            break;
        default:
            break;
        }
    }
    else // code does not exist
    {
        errorRes.message = ERROR_CODE_MSG;
        reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(errorRes);
        reqResult.newHandler = nullptr;
    }
    return reqResult;
}

RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo reqInfo)
{
    RequestResult reqResult;
    LeaveRoomRespone leaveRoom;

    leaveRoom.status = LEAVE_ROOM_SUCCESS_STATUS;
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(leaveRoom);
    m_room.removeUser(m_user);  
    reqResult.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());
    return reqResult;
}

RequestResult RoomMemberRequestHandler::getRoomState(RequestInfo reqInfo)
{
    RequestResult reqResult;
    GetRoomStateRespone getRoomStateRes;

    getRoomStateRes.status = GET_ROOM_STATE_SUCCESS_STATUS;
    getRoomStateRes.hasGameBegun = m_roomManager.getRoomState(m_room.getRoomData().id);
    getRoomStateRes.answerTimeout = m_room.getRoomData().timePerQuestion;
    getRoomStateRes.players = m_room.getAllUsers();
    getRoomStateRes.questionCount = m_room.getRoomData().numOfQuestionsInGame;

    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(getRoomStateRes);
    reqResult.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_room, m_user);

    return reqResult;
} 

RequestResult RoomMemberRequestHandler::getPlayersInRoom(RequestInfo reqInfo)
{
    RequestResult reqResult;
    GetPlayersInRoomRequest getPlayers;
    GetPlayersInRoomRespone getPlayersInRoomRes;

    nlohmann::json clientJsonWithRoomId = nlohmann::json::parse(reqInfo.buffer.begin(), reqInfo.buffer.end());
    clientJsonWithRoomId[ROOM_ID] = m_roomManager.getRoomByName(clientJsonWithRoomId.at(ROOM_ID)).getRoomData().id;
    std::string getPlayersInRoomBufferJson = clientJsonWithRoomId.dump();
    std::vector<unsigned char> getPlayersInRoomBuffer(getPlayersInRoomBufferJson.begin(), getPlayersInRoomBufferJson.end());

    getPlayers = JsonRequestPacketDeserializer::deserializeGetPlayRequest(getPlayersInRoomBuffer);
    getPlayersInRoomRes.players = this->m_roomManager.getRoomById(getPlayers.roomId).getAllUsers();
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(getPlayersInRoomRes);
    reqResult.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_room, m_user);
    return reqResult;
}

RequestResult RoomMemberRequestHandler::Logout(RequestInfo reqInfo)
{
    RequestResult reqResult;
    LogoutRespone logoutRes;

    m_handlerFactory.getLoginManager().logout(m_user.getUsername()); // logout
    logoutRes.status = LOGOUT_SUCCESS_STATUS;
    reqResult.newHandler = m_handlerFactory.createLoginRequestHandler();
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(logoutRes);
    return reqResult;
}
