#include "JsonRequestPacketDeserializer.h"

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(std::vector<unsigned char> buffer)
{
    /*
    * function building login request struct and return it
    */
    LoginRequest loginReq;
    nlohmann::json jsonData = nlohmann::json::parse(buffer.begin(), buffer.end());
    loginReq.username = jsonData.at(USERNAME);
    loginReq.password = jsonData.at(PASSWORD);
    return loginReq;
}

SignUpRequest JsonRequestPacketDeserializer::deserializeSignUpRequest(std::vector<unsigned char> buffer)
{
    /*
    * function building signUp request struct and return it
    */
    SignUpRequest signUpReq;
    nlohmann::json jsonData = nlohmann::json::parse(buffer.begin(), buffer.end());
    signUpReq.username = jsonData.at(USERNAME);
    signUpReq.password = jsonData.at(PASSWORD);
    signUpReq.email = jsonData.at(EMAIL);
    return signUpReq;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayRequest(std::vector<unsigned char> buffer)
{
   /*
   * function building GetPlayersInRoomRequest request struct and return it
   */
    GetPlayersInRoomRequest getPlayersRequest;
    nlohmann::json jsonData = nlohmann::json::parse(buffer.begin(), buffer.end());
    getPlayersRequest.roomId = jsonData.at(ROOM_ID);
	return getPlayersRequest;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(std::vector<unsigned char> buffer)
{
   /*
   * function building JoinRoomRequest request struct and return it
   */
    JoinRoomRequest joinRoomRequest;
    nlohmann::json jsonData = nlohmann::json::parse(buffer.begin(), buffer.end());
    joinRoomRequest.roomId = jsonData.at(ROOM_ID);
	return joinRoomRequest;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(std::vector<unsigned char> buffer)
{
   /*
   * function building CreateRoomRequest request struct and return it
   */
    CreateRoomRequest createRoomRequest;
    nlohmann::json jsonData = nlohmann::json::parse(buffer.begin(), buffer.end());
    createRoomRequest.roomName = jsonData.at(ROOM_NAME);
    createRoomRequest.maxUsers = jsonData.at(MAX_USERS);
    createRoomRequest.questionCount = jsonData.at(QUESTION_COUNT);
    createRoomRequest.answerTimeout = jsonData.at(ANSWER_TIME_OUT);
	return createRoomRequest;
}

GetRoomInfoRequest JsonRequestPacketDeserializer::deserializeGetRoomInfoRequest(std::vector<unsigned char> buffer)
{
    GetRoomInfoRequest roomInfo;
    nlohmann::json jsonData = nlohmann::json::parse(buffer.begin(), buffer.end());
    roomInfo.roomName = jsonData.at(ROOM_NAME);
    return roomInfo;
}

