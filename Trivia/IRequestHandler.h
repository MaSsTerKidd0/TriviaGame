#pragma once

#include <iostream>
#include <ctime>
#include <vector>
#include "Helper.h"

class IRequestHandler;

struct RequestInfo
{
	unsigned int id;
	std::string recevialTime;
	std::vector<unsigned char> buffer; // the buffer is the json
}typedef RequestInfo;

struct RequestResult
{
	std::vector<unsigned char> buffer; // the buffer is the json
	IRequestHandler* newHandler;
}typedef RequestResult;

class IRequestHandler
{
public:
	IRequestHandler() = default;
	~IRequestHandler() = default;

	virtual bool isRequestRelevant(RequestInfo reqInfo) = 0;
	virtual RequestResult handleRequest(RequestInfo reqInfo) = 0;

private:

};




