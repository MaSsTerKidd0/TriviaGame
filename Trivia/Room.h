#pragma once
#include <iostream>
#include <vector>
#include "LoggedUser.h"
#include <mutex>

struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int isActive;
	
}typedef RoomData;

class Room
{
public:
	Room() = default;
	Room(RoomData metadata);
	~Room();

	void addUser(LoggedUser user);
	void removeUser(LoggedUser user);
	RoomData getRoomData();
	std::vector<std::string> getAllUsers();
	void setRoomStateToInActive();

private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
};
