#include "JsonResponsePacketSerializer.h"


/* Function will serialize error respone to the client and return it as unsigned char vector */
std::vector<unsigned char> JsonResponsePacketSerializer::serializeRespone(ErrorRespone errorRes)
{
    nlohmann::json errorJson;
    errorJson[MESSAGE_KEY] = errorRes.message; // create the json error msg respone
    std::string jsonData = errorJson.dump();
    std::vector<unsigned char> responeBuffer(jsonData.begin(), jsonData.end());
    return responeBuffer;
}

/* Function will serialize signup respone to the client and return it as unsigned char vector */
std::vector<unsigned char> JsonResponsePacketSerializer::serializeRespone(SignupRespone signupRes)
{
    return serializeOnlyStatusRespone(signupRes.status);
}

/* Function will serialize login respone to the client and return it as unsigned char vector */
std::vector<unsigned char> JsonResponsePacketSerializer::serializeRespone(LoginRespone loginRes)
{
    return serializeOnlyStatusRespone(loginRes.status);
}

/* Function will serialize Logout respone to the client and return it as unsigned char vector */
std::vector<unsigned char> JsonResponsePacketSerializer::serializeRespone(LogoutRespone logoutRes)
{
    return serializeOnlyStatusRespone(logoutRes.status);
}

// Function is in note because "RoomData" class is not created yet.
std::vector<unsigned char> JsonResponsePacketSerializer::serializeRespone(GetRoomsRespone getRoomsRes)
{
    // Creating the rooms names as string
    std::string roomNames = "";
    for (unsigned int i = 0; i < getRoomsRes.rooms.size(); i++)
    {
        roomNames += getRoomsRes.rooms[i].name + ",";
    }
    roomNames = roomNames.substr(0, roomNames.length() - 1); // remove the last comma sign in the end of the string

    // Creating the json rooms msg respone
    nlohmann::json roomsJson;
    roomsJson[STATUS_KEY] = getRoomsRes.status;
    roomsJson[ROOMS_KEY] = roomNames;
    std::string jsonData = roomsJson.dump();
    std::vector<unsigned char> responeBuffer(jsonData.begin(), jsonData.end());
    return responeBuffer;
}

/* Function will serialize GetPlayersInRoom respone to the client and return it as unsigned char vector */
std::vector<unsigned char> JsonResponsePacketSerializer::serializeRespone(GetPlayersInRoomRespone getPlayersInRoomRes)
{
    // Creating the rooms names as string
    std::string usersNames = "";
    for (unsigned int i = 0; i < getPlayersInRoomRes.players.size(); i++)
    {
        usersNames += getPlayersInRoomRes.players[i] + ",";
    }
    usersNames = usersNames.substr(0, usersNames.length() - 1); // remove the last ', ' signs in the end of the string

    // Creating the json users msg respone
    nlohmann::json usersJson;
    usersJson[PLAYERS_IN_ROOM_KEY] = usersNames;
    std::string jsonData = usersJson.dump();
    std::vector<unsigned char> responeBuffer(jsonData.begin(), jsonData.end());
    return responeBuffer;
}

/* Function will serialize JoinRoom respone to the client and return it as unsigned char vector */
std::vector<unsigned char> JsonResponsePacketSerializer::serializeRespone(JoinRoomRespone JoinRoomRes)
{
    return serializeOnlyStatusRespone(JoinRoomRes.status);
}

/* Function will serialize CreateRoom respone to the client and return it as unsigned char vector */
std::vector<unsigned char> JsonResponsePacketSerializer::serializeRespone(CreateRoomRespone CreateRoomRes)
{
    return serializeOnlyStatusRespone(CreateRoomRes.status);
}

/* Function will serialize GetPersonalStats respone to the client and return it as unsigned char vector */
std::vector<unsigned char> JsonResponsePacketSerializer::serializeRespone(GetPersonalStatsRespone personalStatsRes)
{
    nlohmann::json personalStatsJson;
    
    personalStatsJson[STATUS_KEY] = personalStatsRes.status;
    personalStatsJson[NUM_OF_GAMES] = personalStatsRes.statistics[NUM_OF_GAMES_INDEX];
    personalStatsJson[NUM_OF_RIGHT_ANSWERS] = personalStatsRes.statistics[NUM_OF_RIGHT_ANSWERS_INDEX];
    personalStatsJson[NUM_OF_WRONG_ANSWERS] = personalStatsRes.statistics[NUM_OF_WRONG_ANSWERS_INDEX];
    personalStatsJson[AVERAGE_TIME_FOR_ANSWER] = personalStatsRes.statistics[AVERAGE_TIME_FOR_ANSWER_INDEX];

    std::string jsonData = personalStatsJson.dump();
    std::vector<unsigned char> responeBuffer(jsonData.begin(), jsonData.end());
    return responeBuffer;
}

/* Function will serialize GetHighScore respone to the client and return it as unsigned char vector */
std::vector<unsigned char> JsonResponsePacketSerializer::serializeRespone(GetHighScoreRespone highScoreRes)
{
    nlohmann::json highScoreJson;

    highScoreJson[STATUS_KEY] = highScoreRes.status;
    highScoreJson[HIGH_SCORES_KEY] = highScoreRes.statistics;

    std::string jsonData = highScoreJson.dump();
    std::vector<unsigned char> responeBuffer(jsonData.begin(), jsonData.end());
    return responeBuffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeRespone(RoomData roomData)
{
    nlohmann::json roomInfoJson;

    roomInfoJson[ROOM_NAME_KEY] = roomData.name;
    roomInfoJson[MAX_USERS_KEY] = roomData.maxPlayers;
    roomInfoJson[QUESTION_COUNT_KEY] = roomData.numOfQuestionsInGame;
    roomInfoJson[ANSWER_TIMEOUT_KEY] = roomData.timePerQuestion;

    std::string jsonData = roomInfoJson.dump();
    std::vector<unsigned char> responeBuffer(jsonData.begin(), jsonData.end());
    return responeBuffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeRespone(CloseRoomRespone closeRoomRes)
{
    return serializeOnlyStatusRespone(closeRoomRes.status);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeRespone(StartGameRespone startGameRes)
{
    return serializeOnlyStatusRespone(startGameRes.status);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeRespone(GetRoomStateRespone getRoomStateRes)
{
    nlohmann::json roomStateJson;

    roomStateJson[STATUS_KEY] = getRoomStateRes.status; // create the json room state msg respone
    roomStateJson[HAS_GAME_BEGUN_KEY] = getRoomStateRes.hasGameBegun;
    roomStateJson[PLAYERS_KEY] = getRoomStateRes.players; // VECTORRR
    roomStateJson[QUESTION_COUNT_KEY] = getRoomStateRes.questionCount;
    roomStateJson[ANSWER_TIMEOUT_KEY] = getRoomStateRes.answerTimeout;

    std::string jsonData = roomStateJson.dump();
    std::vector<unsigned char> responeBuffer(jsonData.begin(), jsonData.end());
    return responeBuffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeRespone(LeaveRoomRespone leaveRoomRes)
{
    return serializeOnlyStatusRespone(leaveRoomRes.status);
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeOnlyStatusRespone(unsigned int status)
{
    nlohmann::json json;
    json[STATUS_KEY] = status; // create the json with only status respone
    std::string jsonData = json.dump();
    std::vector<unsigned char> responeBuffer(jsonData.begin(), jsonData.end());
    return responeBuffer;
}

