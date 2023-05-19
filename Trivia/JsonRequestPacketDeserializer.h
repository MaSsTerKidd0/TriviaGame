#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "json.hpp"

#define USERNAME "username"
#define PASSWORD "password"
#define EMAIL "email"
#define ROOM_ID "roomId"
#define ROOM_NAME "roomName"
#define MAX_USERS "maxUsers"
#define QUESTION_COUNT "questionCount"
#define ANSWER_TIME_OUT "answerTimeout"

struct LoginRequest
{
	std::string username;
	std::string password;
}typedef LoginRequest;

struct SignUpRequest
{
	std::string username;
	std::string password;
	std::string email;
}typedef SignUpRequest;

struct GetPlayersInRoomRequest
{
	unsigned int roomId;
}typedef GetPlayersInRoomRequest;

struct JoinRoomRequest
{
	unsigned int roomId;
}typedef JoinRoomRequest;

struct CreateRoomRequest
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
}typedef CreateRoomRequest;

struct GetRoomInfoRequest
{
	std::string roomName;
}typedef GetRoomInfoRequest;

class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(std::vector<unsigned char> buffer);
	static SignUpRequest deserializeSignUpRequest(std::vector<unsigned char> buffer);
	static GetPlayersInRoomRequest deserializeGetPlayRequest(std::vector<unsigned char> buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(std::vector<unsigned char> buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(std::vector<unsigned char> buffer);
	static GetRoomInfoRequest deserializeGetRoomInfoRequest(std::vector<unsigned char> buffer);

private:

};
