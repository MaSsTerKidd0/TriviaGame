#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(Room& room, LoggedUser user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory) : 
	m_room(room), m_user(user), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo reqInfo)
{
	return (reqInfo.id == CLOSE_ROOM_REQUEST || reqInfo.id == START_GAME_REQUEST || reqInfo.id == GET_ROOM_STATE_REQUEST || reqInfo.id == GET_PLAYERS_IN_ROOM_REQUEST || reqInfo.id == LOGOUT_REQUEST);
}

RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo reqInfo)
{
    RequestResult reqResult;
    ErrorRespone errorRes;
    if (isRequestRelevant(reqInfo))
    {
        switch (reqInfo.id)
        {
        case GET_PLAYERS_IN_ROOM_REQUEST:
            reqResult = getPlayersInRoom(reqInfo);
            break;
        case CLOSE_ROOM_REQUEST:
            reqResult = closeRoom(reqInfo);
            break;
        case START_GAME_REQUEST:
            reqResult = startGame(reqInfo);
            break;
        case GET_ROOM_STATE_REQUEST:
            reqResult = getRoomState(reqInfo);
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

RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo reqInfo)
{
    RequestResult reqResult;
    CloseRoomRespone closeRoomRes;
    
    m_room.removeUser(m_user);
    m_room.setRoomStateToInActive();

    while (m_room.getAllUsers().size() != 0) // waiting for the players to disconnect
    {
        Sleep(1000);
    }
    m_roomManager.deleteRoom(m_room.getRoomData().id); // then delete the room

    closeRoomRes.status = CLOSE_ROOM_SUCCESS_STATUS;
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(closeRoomRes);
    reqResult.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());

    return reqResult;
}

RequestResult RoomAdminRequestHandler::startGame(RequestInfo reqInfo)
{
    RequestResult reqResult;
    StartGameRespone startGameRes;

    startGameRes.status = START_GAME_SUCCESS_STATUS;
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(startGameRes);
    reqResult.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_room, m_user);//NewHandler
    

    return reqResult;
}

RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo reqInfo)
{
    RequestResult reqResult;
    GetRoomStateRespone getRoomStateRes;

    getRoomStateRes.status = GET_ROOM_STATE_SUCCESS_STATUS;
    getRoomStateRes.hasGameBegun = m_roomManager.getRoomState(m_room.getRoomData().id);
    getRoomStateRes.answerTimeout = m_room.getRoomData().timePerQuestion;
    getRoomStateRes.players = m_room.getAllUsers();
    getRoomStateRes.questionCount = m_room.getRoomData().numOfQuestionsInGame;
    
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(getRoomStateRes);
    reqResult.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_room, m_user);

    return reqResult;
}

RequestResult RoomAdminRequestHandler::getPlayersInRoom(RequestInfo reqInfo)
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
    reqResult.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_room, m_user);
    return reqResult;
}

RequestResult RoomAdminRequestHandler::Logout(RequestInfo reqInfo)
{
    RequestResult reqResult;
    LogoutRespone logoutRes;

    m_handlerFactory.getLoginManager().logout(m_user.getUsername()); // logout
    logoutRes.status = LOGOUT_SUCCESS_STATUS;
    reqResult.newHandler = m_handlerFactory.createLoginRequestHandler();
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(logoutRes);
    return reqResult;
}