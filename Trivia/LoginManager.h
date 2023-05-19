#pragma once

#include "IDatabase.h"
#include "LoggedUser.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "Codes.h"
#include <mutex>

class LoginManager
{
public:
	LoginManager(IDatabase* database);
	~LoginManager() = default;

	int signup(const std::string username, const std::string password, const std::string email);
	int login(const std::string username, const std::string password);
	void logout(const std::string username);



private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;

	bool isUserLogged(const std::string username);
};