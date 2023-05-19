#include "LoginManager.h"

std::mutex mtxDatabase;
std::mutex mtxLoggedUsers;

LoginManager::LoginManager(IDatabase* database) : m_database(database)
{
}

/*
Function SignUp User By Adding his Personal Information
*/
int LoginManager::signup(const std::string username, const std::string password, const std::string email)
{
	if (!m_database->doesUserExist(username))
	{
		mtxDatabase.lock();
		m_database->addNewUser(username, password, email); // success, username does not exist.
		mtxDatabase.unlock();
		return SIGNUP_SUCCESS_STATUS;
	}
	return SIGNUP_FAILURE_STATUS; // failure, user in that name already exist
}

int LoginManager::login(const std::string username, const std::string password)
{
	if (m_database->doesUserExist(username) && m_database->doesPasswordMatch(username, password) && !isUserLogged(username))
	{
		mtxDatabase.lock();
		m_loggedUsers.push_back(LoggedUser(username)); // success
		mtxDatabase.unlock();
		return LOGIN_SUCCESS_STATUS;
	}
	// else if (m_database->doesUserExist(username) && !m_database->doesPasswordMatch(username, password)) // username is good, password incorrect
	// else if (!m_database->doesUserExist(username) && !m_database->doesPasswordMatch(username, password)) // both wrong		
	return LOGIN_FAILURE_STATUS;
}

/*
Function Logout User By UserName
*/
void LoginManager::logout(const std::string username)
{
	std::vector<LoggedUser>::iterator it = m_loggedUsers.begin();
	for (;it != m_loggedUsers.end(); it++)
	{
		if ((*it).getUsername() == username)
		{
			std::cout << "REMOVE USER: " << (*it).getUsername() << std::endl;
			mtxLoggedUsers.lock();
			m_loggedUsers.erase(it);
			mtxLoggedUsers.unlock();
			break;
		}
	}
}

/*
Function Checks If User Is already Logged
*/
bool LoginManager::isUserLogged(const std::string username)
{
	for (std::vector<LoggedUser>::iterator it = m_loggedUsers.begin(); it != m_loggedUsers.end(); it++)
	{
		if ((*it).getUsername() == username)
		{
			return true;
		}
	}
	return false;
}
