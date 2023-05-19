#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <fstream>
#include <io.h>
#include "json.hpp"

#define TRIVIA_DB_NAME "TriviaDB.sqlite"
#define FAILED_TO_OPEN_DB_ERROR_MSG "Failed to open DB!"
#define QUESTIONS_DATA "Questions_Data.exe"
#define QUESTIONS_DATA_FILE "QuestionsDataFile.txt"
#define APOSTROPHE "'"
#define END_OF_QUERY ");"
#define COMMA_SPACE ", "
#define TOP_3_BEST_PLAYERS 3

#define CREATE_USERS_TABLE "CREATE TABLE IF NOT EXISTS Users(username TEXT PRIMARY KEY NOT NULL, password TEXT NOT NULL, email TEXT NOT NULL);"
#define CREATE_QUESTIONS_TABLE "CREATE TABLE IF NOT EXISTS Questions(ID INTEGER PRIMARY KEY NOT NULL, questionData TEXT NOT NULL);"
#define CREATE_STATISTICS_TABLE "CREATE TABLE IF NOT EXISTS Statistics(username TEXT NOT NULL, numOfGames INTEGER NOT NULL, numOfRightAnswers INTEGER NOT NULL, numOfWrongAnswers INTEGER NOT NULL, averageTimeForAnswer FLOAT NOT NULL, score INTEGER NOT NULL, FOREIGN KEY (username) REFERENCES Users(username));"

#define INSERT_USER "INSERT INTO Users(username, password, email) VALUES ('"
#define INSERT_QUESTION_INFO "INSERT INTO Questions(ID, questionData) VALUES ("
#define INSERT_DEFAULT_STATISTICS "INSERT INTO Statistics(username, numOfGames, numOfRightAnswers, numOfWrongAnswers, averageTimeForAnswer, score) VALUES('"

#define DOES_USER_EXIST "SELECT EXISTS(SELECT * FROM Users WHERE username = '"

#define GET_RANDOM_QUESTIONS "SELECT questionData from Questions ORDER BY RANDOM() LIMIT "
#define GET_PASSWORD_BY_USERNAME "SELECT password FROM USERS WHERE username = '"
#define GET_PASSWORD_BY_USERNAME "SELECT password FROM USERS WHERE username = '"
#define GET_AVERAGE_ANSWER_TIME_BY_USERNAME "SELECT averageTimeForAnswer FROM Statistics WHERE username = '"
#define GET_NUM_OF_CORRECT_ANSWERS_BY_USERNAME "SELECT numOfRightAnswers FROM Statistics WHERE username = '"
#define GET_NUM_OF_WRONG_ANSWERS_BY_USERNAME "SELECT numOfWrongAnswers FROM Statistics WHERE username = '"
#define GET_NUM_OF_GAMES_BY_USERNAME "SELECT numOfGames FROM Statistics WHERE username = '"
#define GET_TOP_3_HIGH_SCORES_NAMES "SELECT username from statistics ORDER BY score LIMIT 3;"
#define GET_TOP_3_HIGH_SCORES "SELECT score from statistics ORDER BY score LIMIT 3;"
#define GET_USER_SCORE "SELECT score from statistics where username = '"

#define RESPONE_CODE "response_code"

class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase() = default;
	~SqliteDatabase();
	virtual bool open();
	virtual void close();

	virtual bool doesUserExist(const std::string username);
	virtual bool doesPasswordMatch(const std::string username, const std::string password);
	virtual void addNewUser(const std::string username, const std::string password, const std::string email);
	virtual std::vector<std::string> getQuestion(int numOfQuestions);
	virtual float getPlayerAverageAnswerTime(const std::string username);
	virtual int getNumOfCorrectAnswers(const std::string username);
	virtual int getNumOfTotalAnswers(const std::string username);
	virtual int getNumOfPlayerGames(const std::string username);
	virtual std::vector<std::string> getHighScores();
	virtual int getPlayerScore(const std::string username);

private:
	sqlite3* _db;

	void executeQuery(const char* sqlStatement);

	int executeQueryWithReturnedInteger(const char* sqlStatement);
	float executeQueryWithReturnedFloat(const char* sqlStatement);
	std::string executeQueryWithReturnedString(const char* sqlStatement);
	std::vector<std::string> executeQueryWithReturnedListOfStrings(const char* sqlStatement);

	static int getIntegerCallBack(void* data, int argc, char** argv, char** azColName);
	static int getFloatCallBack(void* data, int argc, char** argv, char** azColName);
	static int getStringCallBack(void* data, int argc, char** argv, char** azColName);
	static int getListOfStringsCallBack(void* data, int argc, char** argv, char** azColName);

	void receiveQuestionsData();
	bool isQuestionsTransferProperly(std::string respone);
	void insertQuestionToTable(int id, std::string question_data);

};