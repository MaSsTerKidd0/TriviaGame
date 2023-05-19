#pragma once

#include <iostream>
#include <string>
#include <vector>

class IDatabase
{
public:
	virtual ~IDatabase() = default;
	virtual bool open() = 0;
	virtual void close() = 0;

	virtual bool doesUserExist(const std::string username) = 0;
	virtual bool doesPasswordMatch(const std::string username, const std::string password) = 0;
	virtual void addNewUser(const std::string username, const std::string password, const std::string email) = 0;
	virtual std::vector<std::string> getQuestion(int numOfQuestions) = 0;
	virtual float getPlayerAverageAnswerTime(const std::string username) = 0;
	virtual int getNumOfCorrectAnswers(const std::string username) = 0;
	virtual int getNumOfTotalAnswers(const std::string username) = 0;
	virtual int getNumOfPlayerGames(const std::string username) = 0;
	virtual std::vector<std::string> getHighScores() = 0;
	virtual int getPlayerScore(const std::string username) = 0;
};