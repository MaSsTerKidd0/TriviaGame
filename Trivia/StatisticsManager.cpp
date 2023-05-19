#include "StatisticsManager.h"

std::mutex mtxUserStats;

StatisticsManager::StatisticsManager(IDatabase* database) : 
	m_database(database)
{
}

std::vector<std::string> StatisticsManager::getHighScore()
{
	return m_database->getHighScores();
}

/*
Function gets User Statistics by using sql statements from differnt function
returns the userStats as vector of strings
*/
std::vector<std::string> StatisticsManager::getUserStatistics(const std::string username)
{
	mtxUserStats.lock();
	int numOfGames = m_database->getNumOfPlayerGames(username);
	int correctAnswers = m_database->getNumOfCorrectAnswers(username);
	int wrongAnswers = m_database->getNumOfTotalAnswers(username) - correctAnswers;
	float avgTimePerAnswer = m_database->getPlayerAverageAnswerTime(username);
	int score = m_database->getPlayerScore(username);
	std::vector<std::string> userStats;

	userStats.push_back(std::to_string(numOfGames));
	userStats.push_back(std::to_string(correctAnswers));
	userStats.push_back(std::to_string(wrongAnswers));
	userStats.push_back(std::to_string(avgTimePerAnswer));
	mtxUserStats.unlock();
	return userStats;
}
