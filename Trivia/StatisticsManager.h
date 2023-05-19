#pragma once

#include "IDatabase.h"
#include <vector>
#include <string>
#include <mutex>

class StatisticsManager
{
public:
	StatisticsManager(IDatabase* database);
	~StatisticsManager() = default;

	std::vector<std::string> getHighScore();
	std::vector<std::string> getUserStatistics(const std::string username);

private:
	IDatabase* m_database;
};