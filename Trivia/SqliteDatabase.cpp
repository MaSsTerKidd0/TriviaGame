#include "SqliteDatabase.h"

/* Function return true if user exist, else - false */
bool SqliteDatabase::doesUserExist(const std::string username)
{
	std::string sqlQueryWithParams = DOES_USER_EXIST + username + APOSTROPHE + END_OF_QUERY;
	int isUserExist = executeQueryWithReturnedInteger(sqlQueryWithParams.c_str());
	return isUserExist == 1;
}

/* password check if the password that the user enter match to the one he has. true if it has, else - false */
bool SqliteDatabase::doesPasswordMatch(const std::string username, const std::string password)
{
	std::string sqlQueryWithParams = GET_PASSWORD_BY_USERNAME + username + APOSTROPHE + ";";
	std::string returnedPassword = executeQueryWithReturnedString(sqlQueryWithParams.c_str());
	return password == returnedPassword;
}

/* Function create query to create user and send it to the database */
void SqliteDatabase::addNewUser(const std::string username, const std::string password, const std::string email)
{
	std::string sqlQueryWithParams = INSERT_USER + username + APOSTROPHE + COMMA_SPACE + APOSTROPHE + password + APOSTROPHE + COMMA_SPACE + APOSTROPHE + email + APOSTROPHE + END_OF_QUERY;
	executeQuery(sqlQueryWithParams.c_str());
	sqlQueryWithParams = INSERT_DEFAULT_STATISTICS + username + APOSTROPHE + COMMA_SPACE + "0, 0, 0, 0, 0);";
	executeQuery(sqlQueryWithParams.c_str());
}

/*function get 5 random questions from questions Table using sql statement*/
std::vector<std::string> SqliteDatabase::getQuestion(int numOfQuestions)
{
	std::string sqlGetRandomQuestions = GET_RANDOM_QUESTIONS + std::to_string(numOfQuestions) + ";";
	return executeQueryWithReturnedListOfStrings(sqlGetRandomQuestions.c_str());
}

/*function get PlayerAverageAnswerTime from statistics Table using sql statement*/
float SqliteDatabase::getPlayerAverageAnswerTime(const std::string username)
{
	std::string sqlQueryWithParams = GET_AVERAGE_ANSWER_TIME_BY_USERNAME + username + APOSTROPHE + ";";
	return executeQueryWithReturnedFloat(sqlQueryWithParams.c_str());
}

/*function get NumOfCorrectAnswers from statistics Table using sql statement*/
int SqliteDatabase::getNumOfCorrectAnswers(const std::string username)
{
	std::string sqlQueryWithParams = GET_NUM_OF_CORRECT_ANSWERS_BY_USERNAME + username + APOSTROPHE + ";";
	return executeQueryWithReturnedInteger(sqlQueryWithParams.c_str());
}

/*function get NumOfTotalAnswers from statistics Table using sql statement*/
int SqliteDatabase::getNumOfTotalAnswers(const std::string username)
{
	std::string sqlQueryWithParams = GET_NUM_OF_WRONG_ANSWERS_BY_USERNAME + username + APOSTROPHE + ";";
	return (getNumOfCorrectAnswers(username) + executeQueryWithReturnedInteger(sqlQueryWithParams.c_str()));
}

/*function get NumOfPlayerGames from statistics Table using sql statement*/
int SqliteDatabase::getNumOfPlayerGames(const std::string username)
{
	std::string sqlQueryWithParams = GET_NUM_OF_GAMES_BY_USERNAME + username + APOSTROPHE + ";";
	return executeQueryWithReturnedInteger(sqlQueryWithParams.c_str());
}

/*function get the five top Scored player Names from statistics Table using sql statement*/
std::vector<std::string> SqliteDatabase::getHighScores()
{
	std::vector<std::string> names = executeQueryWithReturnedListOfStrings(std::string(GET_TOP_3_HIGH_SCORES_NAMES).c_str());
	std::vector<std::string> scores = executeQueryWithReturnedListOfStrings(std::string(GET_TOP_3_HIGH_SCORES).c_str());
	std::vector<std::string> namesAndScore;
	int fillVector = 0;

	if (names.size() < TOP_3_BEST_PLAYERS)
	{
		if (names.size() == 0) fillVector = 3;
		if (names.size() == 1) fillVector = 2;
		if (names.size() == 2) fillVector = 1;
		for (int i = 0; i < TOP_3_BEST_PLAYERS - fillVector; i++)
		{
			namesAndScore.push_back(names[i]);
			namesAndScore.push_back(scores[i]);
		}
		for (int i = 0; i < fillVector; i++)
		{
			namesAndScore.push_back(" ---");
			namesAndScore.push_back("None");
		}
	}
	else
	{
		for (int i = 0; i < TOP_3_BEST_PLAYERS; i++)
		{
			namesAndScore.push_back(names[i]);
			namesAndScore.push_back(scores[i]);
		}
	}

	return namesAndScore;
}

/*function get PlayerScore by name from statistics Table using sql statement*/
int SqliteDatabase::getPlayerScore(const std::string username)
{
	std::string sqlQueryWithParams = GET_USER_SCORE + username + APOSTROPHE + ";";
	return executeQueryWithReturnedInteger(sqlQueryWithParams.c_str());
}

SqliteDatabase::~SqliteDatabase()
{
	close();
}

/* Function create database if it doesn't exist, else - open the existing database. */
bool SqliteDatabase::open()
{
	std::string dbName = TRIVIA_DB_NAME; // "TriviaDB.sqlite"

	int doesFileExist = _access(dbName.c_str(), 0);
	int res = sqlite3_open(dbName.c_str(), &_db);
	if (res != SQLITE_OK)
	{
		_db = nullptr;
		std::cout << FAILED_TO_OPEN_DB_ERROR_MSG << " Error: " << res << std::endl;
		return false;
	}
	if (doesFileExist != 0) // init database, creating all the needed tables
	{
		executeQuery(CREATE_USERS_TABLE);
		executeQuery(CREATE_QUESTIONS_TABLE);
		receiveQuestionsData();
		executeQuery(CREATE_STATISTICS_TABLE);
	}
	return true;

}

/* Function close the opened database */
void SqliteDatabase::close()
{
	sqlite3_close(_db);
	_db = nullptr;
}

/* Function receive sql statement to send to the database and check if it has been sent succesfully */
void SqliteDatabase::executeQuery(const char* sqlStatement)
{
	char** errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlStatement, nullptr, nullptr, errMessage);
	if (res != SQLITE_OK)
	{
		std::cout << "Error Ocurred: " << res << " -> " << errMessage << std::endl;
		std::exit(1);
	}
}

/* Function receive sql statement to send to the database and check if it has been sent succesfully.
return: integer from the query */
int SqliteDatabase::executeQueryWithReturnedInteger(const char* sqlStatement)
{
	char** errMessage = nullptr;
	int num = 0;
	int res = sqlite3_exec(_db, sqlStatement, getIntegerCallBack, &num, errMessage);
	if (res != SQLITE_OK)
	{
		std::cout << "Error Ocurred: " << res << " -> " << errMessage << std::endl;
		std::exit(1);
	}
	return num;
}

/* Function receive sql statement to send to the database and check if it has been sent succesfully.
return: string from the query */
std::string SqliteDatabase::executeQueryWithReturnedString(const char* sqlStatement)
{
	char** errMessage = nullptr;
	std::string str = "";
	int res = sqlite3_exec(_db, sqlStatement, getStringCallBack, &str, errMessage);
	if (res != SQLITE_OK)
	{
		std::cout << "Error Ocurred: " << res << " -> " << errMessage << std::endl;
		std::exit(1);
	}
	return str;
	return 0;
}

/*
Function executeQueryWithReturnedListOfStrings using getListOfStringsCallBack functions
*/
std::vector<std::string> SqliteDatabase::executeQueryWithReturnedListOfStrings(const char* sqlStatement)
{
	std::vector<std::string> stringList;
	char** errMessage = nullptr;
	int res = sqlite3_exec(_db, sqlStatement, getListOfStringsCallBack, &stringList, errMessage);
	if (res != SQLITE_OK)
	{
		std::cout << "Error Ocurred: " << res << " -> " << errMessage << std::endl;
		std::exit(1);
	}
	return stringList;
}

/*function executeQueryWithReturnedFloat using getFloatCallBack function*/
float SqliteDatabase::executeQueryWithReturnedFloat(const char* sqlStatement)
{
	char** errMessage = nullptr;
	float floatNum = 0;
	int res = sqlite3_exec(_db, sqlStatement, getFloatCallBack, &floatNum, errMessage);
	if (res != SQLITE_OK)
	{
		std::cout << "Error Ocurred: " << res << " -> " << errMessage << std::endl;
		std::exit(1);
	}
	return floatNum;
}

/* Function get number from the SELECT sql query */
int SqliteDatabase::getIntegerCallBack(void* data, int argc, char** argv, char** azColName)
{
	int* num = (int*)data;
	*num = atoi(argv[0]);
	return 0;
}

/* Function get string from the SELECT sql query */
int SqliteDatabase::getStringCallBack(void* data, int argc, char** argv, char** azColName)
{
	std::string* str = (std::string*)data;
	*str = argv[0];
	return 0;
}

/* Function get vector Of strings from the SELECT sql query */
int SqliteDatabase::getListOfStringsCallBack(void* data, int argc, char** argv, char** azColName)
{
	std::vector<std::string>* questionsData = (std::vector<std::string>*)data;
	questionsData->push_back(argv[0]);
	return 0;
}

/* Function get Float from the SELECT sql query */
int SqliteDatabase::getFloatCallBack(void* data, int argc, char** argv, char** azColName)
{
	float* num = (float*)data;
	*num = std::stof(argv[0]);
	return 0;
}

/*
function receiving 50 questions from web using python exe file
*/
void SqliteDatabase::receiveQuestionsData()
{
	system(QUESTIONS_DATA);

	std::ifstream questionsDataFile(QUESTIONS_DATA_FILE);
	std::string line;

	unsigned int counter = 0;
	bool dataOk = false;

	if (questionsDataFile.is_open())
	{

		while (getline(questionsDataFile, line))
		{
			if (counter == 0)
				dataOk = isQuestionsTransferProperly(line);
			if (dataOk && counter > 0)
			{
				insertQuestionToTable(counter, line);
			}
			counter++;
		}
		questionsDataFile.close();
	}
}

/* Function Checks if QuestionsTransferProperly from the Web to the Server*/
bool SqliteDatabase::isQuestionsTransferProperly(std::string respone)
{
	nlohmann::json jsonData = nlohmann::json::parse(respone);
	if (jsonData.at(RESPONE_CODE) == 0)
		return true;
	return false;
}
/*function insert questions to questions table using sql query*/
void SqliteDatabase::insertQuestionToTable(int id, std::string question_data)
{
	std::string sqlQueryInsert = INSERT_QUESTION_INFO + std::to_string(id) + COMMA_SPACE + APOSTROPHE + question_data + APOSTROPHE + END_OF_QUERY;
	executeQuery(sqlQueryInsert.c_str());
}