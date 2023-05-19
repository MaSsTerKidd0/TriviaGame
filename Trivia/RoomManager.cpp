#include "RoomManager.h"

std::mutex mtxRooms;

void RoomManager::createRoom(LoggedUser user, RoomData roomData)
{
    mtxRooms.lock();
    this->m_rooms.insert(std::pair<unsigned int, Room>(roomData.id, Room(roomData)));
    mtxRooms.unlock();
}

void RoomManager::deleteRoom(unsigned int id)
{
    //check if there are any rooms to erase
    if (this->m_rooms.size() > 0)
    {
        mtxRooms.lock();
        this->m_rooms.erase(id);
        mtxRooms.unlock();
    }
}
    
unsigned int RoomManager::getRoomState(unsigned int id)
{
    return this->m_rooms[id].getRoomData().isActive;
}

/* function runs loop to push all rooms data into rooms vector */
std::vector<RoomData> RoomManager::getRooms()
{
    std::vector<RoomData> rooms;
    mtxRooms.lock();
    for (auto room : this->m_rooms)
    {
        rooms.push_back(room.second.getRoomData());
    }
    mtxRooms.unlock();
    return rooms;
}

/* function Get Room By Receiving The Room Id*/
Room& RoomManager::getRoomById(unsigned int id)
{
    return m_rooms[id];
}

Room& RoomManager::getRoomByName(std::string roomName)
{
    std::map<unsigned int, Room>::iterator it;

    for (it = m_rooms.begin(); it != m_rooms.end(); it++)
    {
        if (it->second.getRoomData().name == roomName)
        {
            return it->second;
        }
    }
}
