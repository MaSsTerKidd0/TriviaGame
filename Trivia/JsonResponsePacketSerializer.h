#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "json.hpp"
#include "Room.h"

#define MESSAGE_KEY "message"
#define STATUS_KEY "status"
#define ROOMS_KEY "rooms"
#define PLAYERS_IN_ROOM_KEY "playersInRoom"

#define USER_STATISTICS_KEY "userStatistics"
#define HIGH_SCORES_KEY "highScores"

#define NUM_OF_GAMES "number_of_games"
#define NUM_OF_RIGHT_ANSWERS "number_of_right_answers"
#define NUM_OF_WRONG_ANSWERS "number_of_wrong_answers"
#define AVERAGE_TIME_FOR_ANSWER "average_time_for_answer"

#define NUM_OF_GAMES_INDEX 0
#define NUM_OF_RIGHT_ANSWERS_INDEX 1
#define NUM_OF_WRONG_ANSWERS_INDEX 2
#define AVERAGE_TIME_FOR_ANSWER_INDEX 3

#define SCORE_KEY "Score"

#define ROOM_NAME_KEY "roomName"
#define MAX_USERS_KEY "maxUsers"
#define QUESTION_COUNT_KEY "questionCount"
#define ANSWER_TIMEOUT_KEY "answerTimeout"

#define HAS_GAME_BEGUN_KEY "hasGameBegun"
#define PLAYERS_KEY "players"



struct LoginRespone
{
	unsigned int status;
}typedef LoginRespone;

struct SignupRespone
{
	unsigned int status;
}typedef SignupRespone;

struct ErrorRespone
{
	std::string message;
}typedef ErrorRespone;

struct LogoutRespone 
{
	unsigned int status;
}typedef LogoutRespone;

struct GetRoomsRespone 
{
	unsigned int status;
	std::vector<RoomData> rooms;
}typedef GetRoomsRespone;

struct GetPlayersInRoomRespone
{
	std::vector<std::string> players;
}typedef GetPlayersInRoomRespone;

struct GetHighScoreRespone
{
	unsigned int status;
	std::vector<std::string> statistics;
}typedef GetHighScoreRespone;

struct GetPersonalStatsRespone
{
	unsigned int status;
	std::vector<std::string> statistics;
}typedef GetPersonalStatsRespone;

struct JoinRoomRespone
{
	unsigned int status;
}typedef JoinRoomRespone;

struct CreateRoomRespone
{
	unsigned int status;
}typedef CreateRoomRespone;

struct CloseRoomRespone
{
	unsigned int status;
}typedef CloseRoomRespone;

struct StartGameRespone
{
	unsigned int status;
}typedef StartGameRespone;

struct GetRoomStateRespone
{
	unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeout;
}typedef GetRoomStateRespone;

struct LeaveRoomRespone
{
	unsigned int status;
}typedef LeaveRoomRespone;

/*
	Class will have static function that will serialize Respones to the client.
*/

class JsonResponsePacketSerializer
{
public:
	static std::vector<unsigned char> serializeRespone(ErrorRespone errorRes);
	static std::vector<unsigned char> serializeRespone(SignupRespone signupRes);
	static std::vector<unsigned char> serializeRespone(LoginRespone loginRes);
	static std::vector<unsigned char> serializeRespone(LogoutRespone logoutRes);
	static std::vector<unsigned char> serializeRespone(GetRoomsRespone getRoomsRes);
	static std::vector<unsigned char> serializeRespone(GetPlayersInRoomRespone getPlayersInRoomRes);
	static std::vector<unsigned char> serializeRespone(JoinRoomRespone joinRoomRes);
	static std::vector<unsigned char> serializeRespone(CreateRoomRespone createRoomRes);
	static std::vector<unsigned char> serializeRespone(GetPersonalStatsRespone personalStatsRes);
	static std::vector<unsigned char> serializeRespone(GetHighScoreRespone highScoreRes);
	static std::vector<unsigned char> serializeRespone(RoomData roomData);

	static std::vector<unsigned char> serializeRespone(CloseRoomRespone closeRoomRes);
	static std::vector<unsigned char> serializeRespone(StartGameRespone startGameRes);
	static std::vector<unsigned char> serializeRespone(GetRoomStateRespone getRoomStateRes);
	static std::vector<unsigned char> serializeRespone(LeaveRoomRespone leaveRoomRes);

private:
	static std::vector<unsigned char> serializeOnlyStatusRespone(unsigned int status);


};