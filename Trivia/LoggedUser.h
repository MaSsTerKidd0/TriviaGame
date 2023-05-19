#pragma once

#include <iostream>

class LoggedUser
{
public:
	LoggedUser();
	LoggedUser(const std::string username);
	~LoggedUser() = default;

	std::string getUsername() const;

private:
	std::string m_username;
};