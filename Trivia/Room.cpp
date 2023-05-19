#include "Room.h"

std::mutex mtxMUserVector;

Room::Room(RoomData metadata)
{
	this->m_metadata = metadata;
}

Room::~Room()
{
}

void Room::addUser(LoggedUser user)
{
	mtxMUserVector.lock();
    this->m_users.push_back(user);
	mtxMUserVector.unlock();
}

/*functions runs loop using iterator in order to find the right user to remove from the vector*/
void Room::removeUser(LoggedUser user)
{
	mtxMUserVector.lock();
	for (std::vector<LoggedUser>::iterator it = m_users.begin(); it != m_users.end(); it++)
	{
		if ((*it).getUsername() == user.getUsername())
		{
			std::cout << "REMOVE USER: " << (*it).getUsername() << std::endl;
			m_users.erase(it);
			break;
		}
	}
	mtxMUserVector.unlock();
}

RoomData Room::getRoomData()
{
	return this->m_metadata;
}

/*function using iterator loop in order to push all of loggedUsers Names*/
std::vector<std::string> Room::getAllUsers()
{
	mtxMUserVector.lock();
	std::vector<std::string> usersNames;
	for (std::vector<LoggedUser>::iterator it = m_users.begin(); it != m_users.end(); it++)
	{
		usersNames.push_back(it->getUsername());
	}
	mtxMUserVector.unlock();

    return usersNames;
}

void Room::setRoomStateToInActive()
{
	m_metadata.isActive = false;
}
