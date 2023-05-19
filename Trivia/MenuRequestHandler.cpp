
#include "MenuRequestHandler.h"

static int countRooms = 0;

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& handlerFactory, RoomManager& roomManager, StatisticsManager& statisticsManager, const std::string username) :
    m_handlerFactory(handlerFactory), m_roomManager(roomManager), m_statisticsManager(statisticsManager), m_user(LoggedUser(username))
{
}

bool MenuRequestHandler::isRequestRelevant(RequestInfo reqInfo)
{
	return (reqInfo.id == CREATE_ROOM_REQUEST || reqInfo.id == GET_ROOMS_REQUEST
        || reqInfo.id == GET_PLAYERS_IN_ROOM_REQUEST || reqInfo.id == JOIN_ROOM_REQUEST 
        || reqInfo.id == GET_PERSONAL_STATISTICS_REQUEST || reqInfo.id == GET_BEST_STATISTICS_REQUEST || reqInfo.id == LOGOUT_REQUEST || reqInfo.id == ROOM_INFO_REQUEST);
}

/*
Function Gets ReqInfo And transfer The Info to the proper Function from the ReqInfo Id.
*/
RequestResult MenuRequestHandler::handleRequest(RequestInfo reqInfo)
{
    RequestResult reqResult;
    ErrorRespone errorRes;
    if (isRequestRelevant(reqInfo))
    {
        switch (reqInfo.id)
        {
            case LOGOUT_REQUEST:
                reqResult = signout(reqInfo);
                break;
            case GET_ROOMS_REQUEST:
                reqResult = getRooms(reqInfo);
                break;
            case GET_PLAYERS_IN_ROOM_REQUEST:
                reqResult = getPlayersInRoom(reqInfo);
                break;
            case GET_PERSONAL_STATISTICS_REQUEST:
                reqResult = getPersonalStats(reqInfo);
                break;
            case GET_BEST_STATISTICS_REQUEST:
                reqResult = getHighScore(reqInfo);
                break;
            case JOIN_ROOM_REQUEST:
                reqResult = joinRoom(reqInfo);
                break;
            case CREATE_ROOM_REQUEST:
                reqResult = createRoom(reqInfo);
                break;
            case ROOM_INFO_REQUEST:
                reqResult = getRoomSettings(reqInfo);
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

/*
Function signOut A user And Update Status
*/
RequestResult MenuRequestHandler::signout(RequestInfo reqInfo)
{
    RequestResult reqResult;
    LogoutRespone logoutRes;

    m_handlerFactory.getLoginManager().logout(m_user.getUsername()); // logout
    logoutRes.status = LOGOUT_SUCCESS_STATUS;
    reqResult.newHandler = m_handlerFactory.createLoginRequestHandler();
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(logoutRes);
    return reqResult;
}

/*
Function get Rooms And Update Status
*/
RequestResult MenuRequestHandler::getRooms(RequestInfo reqInfo)
{
    RequestResult reqResult;
    GetRoomsRespone roomsRes;
    roomsRes.rooms = this->m_roomManager.getRooms();
    roomsRes.status = GET_ROOMS_SUCCESS_STATUS;
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(roomsRes);
    reqResult.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());
    return reqResult;
}

/*
Function getPlayersInRoom And Update Status
*/
RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo reqInfo)
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
    reqResult.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());
    return reqResult;
}

/*
Function getPersonalStats And Update Status
*/
RequestResult MenuRequestHandler::getPersonalStats(RequestInfo reqInfo)
{
    RequestResult reqResult;
    GetPersonalStatsRespone currentUserStats;

    currentUserStats.statistics = this->m_statisticsManager.getUserStatistics(this->m_user.getUsername());;
    currentUserStats.status = GET_PERSONAL_STATISTICS_SUCCESS_STATUS;
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(currentUserStats);
    reqResult.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());
    return reqResult;
}

/*
Function getHighScore And Update Status
*/
RequestResult MenuRequestHandler::getHighScore(RequestInfo reqInfo)
{
    RequestResult reqResult;
    GetHighScoreRespone currentHighScore;
 
    currentHighScore.statistics = this->m_statisticsManager.getHighScore();
    currentHighScore.status = GET_BEST_STATISTICS_SUCCESS_STATUS;
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(currentHighScore);
    reqResult.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());
    return reqResult;
}

/*
Function joinRoom And Update Status
*/
RequestResult MenuRequestHandler::joinRoom(RequestInfo reqInfo)
{
    RequestResult reqResult;
    JoinRoomRespone joinRoomRespone;
    RoomData roomData;

    nlohmann::json clientJsonWithRoomId = nlohmann::json::parse(reqInfo.buffer.begin(), reqInfo.buffer.end());
    clientJsonWithRoomId[ROOM_ID] = m_roomManager.getRoomByName(clientJsonWithRoomId.at(ROOM_ID)).getRoomData().id;
    std::string roomIdJson = clientJsonWithRoomId.dump();
    std::vector<unsigned char> JoinRoomBuffer(roomIdJson.begin(), roomIdJson.end());

    JoinRoomRequest joinRoomRequest = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(JoinRoomBuffer);

    if (m_roomManager.getRoomById(joinRoomRequest.roomId).getAllUsers().size() < m_roomManager.getRoomById(joinRoomRequest.roomId).getRoomData().maxPlayers)
    {
        m_roomManager.getRoomById(joinRoomRequest.roomId).addUser(m_user);
        joinRoomRespone.status = JOIN_ROOM_SUCCESS_STATUS;
        reqResult.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_roomManager.getRoomById(joinRoomRequest.roomId), m_user);
    }
    else // else there is no place in room
    {
        joinRoomRespone.status = JOIN_ROOM_FAILURE_STATUS;
        reqResult.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());
    }
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(joinRoomRespone);
    return reqResult;
}
/*
Function getPersonalStats And Update Status
*/
RequestResult MenuRequestHandler::createRoom(RequestInfo reqInfo)
{
    RequestResult reqResult;
    CreateRoomRequest createRoomRequest = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(reqInfo.buffer);
    CreateRoomRespone createRoomRes;
    RoomData roomData;

    
    if (createRoomRequest.maxUsers > 0 && createRoomRequest.questionCount > 0 
        && !createRoomRequest.roomName.empty() && createRoomRequest.answerTimeout > 0)
    {
        roomData.id = ++countRooms;
        roomData.isActive = true;
        roomData.maxPlayers = createRoomRequest.maxUsers;
        roomData.name = createRoomRequest.roomName;
        roomData.numOfQuestionsInGame = createRoomRequest.questionCount;
        roomData.timePerQuestion = createRoomRequest.answerTimeout;
        m_roomManager.createRoom(m_user, roomData);
        m_roomManager.getRoomById(roomData.id).addUser(m_user);
        createRoomRes.status = CREATE_ROOM_SUCCESS_STATUS;
        Room room = m_roomManager.getRoomById(roomData.id);
        reqResult.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_roomManager.getRoomById(roomData.id), m_user);
    }
    else
    {
        createRoomRes.status = CREATE_ROOM_FAILURE_STATUS;
        reqResult.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());
    }
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(createRoomRes);
    return reqResult;
}

RequestResult MenuRequestHandler::getRoomSettings(RequestInfo reqInfo)
{
    RequestResult reqResult;
    GetRoomInfoRequest roomInfoRequest = JsonRequestPacketDeserializer::deserializeGetRoomInfoRequest(reqInfo.buffer);
    RoomData roomData = m_roomManager.getRoomByName(roomInfoRequest.roomName).getRoomData();
    reqResult.buffer = JsonResponsePacketSerializer::serializeRespone(roomData);
    reqResult.newHandler = m_handlerFactory.createMenuRequestHandler(m_user.getUsername());
    return reqResult;
}

