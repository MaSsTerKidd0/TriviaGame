#pragma once
#include "Room.h"
#include <map>
#include <mutex>

class RoomManager
{
public:
	RoomManager() = default;
	~RoomManager() = default;
	void createRoom(LoggedUser user, RoomData roomData);
	void deleteRoom(unsigned int id);
	unsigned int getRoomState(unsigned int id);
	std::vector<RoomData> getRooms();
	Room& getRoomById(unsigned int id);
	Room& getRoomByName(std::string roomName);

private:
	std::map<unsigned int, Room> m_rooms;

};