#include "Communicator.h"
#include <conio.h>
#include <chrono>
#include <ctime>
#pragma warning(disable : 4996)

Communicator::Communicator(RequestHandlerFactory& handlerFactory):
	m_handlerFactory(handlerFactory)
{
	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
}

Communicator::~Communicator()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		closesocket(m_serverSocket);
	}
	catch (...) {}
}

void Communicator::startHandleRequests()
{
	bindAndListen();

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		std::cout << "Waiting for client connection request" << std::endl;
		accept();
	}
}

void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(PORT); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// again stepping out to the global namespace
	// Connects between the socket and the configuration (port and etc..)
	if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << PORT << std::endl;
}

void Communicator::accept()
{
	// notice that we step out to the global namespace
	// for the resolution of the function accept

	// this accepts the client and create a specific socket from server to this client
	SOCKET client_socket = ::accept(m_serverSocket, NULL, NULL);

	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	std::cout << "Client accepted. Server and client can speak" << std::endl;

	// the function that handle the conversation with the client
	// each client have socket
	std::thread handleClientThread(&Communicator::handleNewClient, this, client_socket);
	handleClientThread.detach();
}



void Communicator::buildRequestInfoStruct(RequestInfo& reqInfo, SOCKET socket)
{
	time_t timeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	reqInfo.recevialTime = ctime(&timeNow);
	reqInfo.id = Helper::getMessageTypeCode(socket);
	int dataLength = Helper::getIntPartFromSocket(socket, DATA_LENGTH); // 4 is the num of bytes of the data length
	std::string jsonAsStr = Helper::getStringPartFromSocket(socket, dataLength);
	reqInfo.buffer = std::vector<unsigned char>(jsonAsStr.begin(), jsonAsStr.end());
}

/*
Function build the respone buffer to return to the client
*/
std::string Communicator::buildResponeBuffer(RequestResult result, int responeCode)
{
	std::string respone;
	unsigned char* dataLen = new unsigned char[sizeof(int)]();
	(*((int*)dataLen)) = result.buffer.size();  // the length to unsigned char array
	respone += responeCode;
	for (int i = 1; i < 5; i++) // move the size in bytes to the full respone
	{
		respone += dataLen[sizeof(int) - i];
	}
	respone += std::string(result.buffer.begin(), result.buffer.end()); // add the json data
	delete[] dataLen;
	return respone;
}

/*
Function check request type and return it respone
*/
int Communicator::getResponeCode(int requestId)
{
	switch (requestId)
	{
	case SIGNUP_REQUEST:
		return SIGNUP_RESPONE;
		break;
	case LOGIN_REQUEST:
		return LOGIN_RESPONE;
		break;
	case CREATE_ROOM_REQUEST:
		return CREATE_ROOM_RESPONE;
		break;
	case GET_ROOMS_REQUEST:
		return GET_ROOMS_RESPONE;
		break;
	case GET_PLAYERS_IN_ROOM_REQUEST:
		return GET_PLAYERS_IN_ROOM_RESPONE;
		break;
	case JOIN_ROOM_REQUEST:
		return JOIN_ROOM_RESPONE;
		break;
	case GET_PERSONAL_STATISTICS_REQUEST:
		return GET_PERSONAL_STATISTICS_RESPONE;
		break;
	case GET_BEST_STATISTICS_REQUEST:
		return GET_BEST_STATISTICS_RESPONE;
		break;
	case LOGOUT_REQUEST:
		return LOGOUT_RESPONE;
		break;
	case ROOM_INFO_REQUEST:
		return ROOM_INFO_RESPONE;
		break;
	case CLOSE_ROOM_REQUEST:
		return CLOSE_ROOM_RESPONE;
		break;
	case START_GAME_REQUEST:
		return START_GAME_RESPONE;
		break;
	case LEAVE_ROOM_REQUEST:
		return LEAVE_ROOM_RESPONE;
		break;
	case GET_ROOM_STATE_REQUEST:
		return GET_ROOM_STATE_RESPONE;
		break;
	default:
		return ERROR_RESPONE_CODE;
		break;
	}
}

/*	
Function communicate with the client
*/
void Communicator::handleNewClient(SOCKET clientSocket)
{
	try
	{
		RequestInfo reqInfo;
		m_clients.insert(std::pair<SOCKET, IRequestHandler*>(clientSocket, m_handlerFactory.createLoginRequestHandler())); // adding new user to map

		while (true)
		{
			buildRequestInfoStruct(reqInfo, clientSocket);
			RequestResult result = m_clients[clientSocket]->handleRequest(reqInfo);

			// update handler
			delete m_clients[clientSocket];
			m_clients[clientSocket] = result.newHandler;

			int responeCode = getResponeCode(reqInfo.id);
			std::string responeBuffer = buildResponeBuffer(result, responeCode);

			//if (responeCode == CLOSE_ROOM_RESPONE || responeCode == START_GAME_RESPONE)
			//{
			//	broadcastMsg()
			//}

			std::cout << "Server msg respone is: " << responeBuffer << std::endl;
			send(clientSocket, responeBuffer.c_str(), responeBuffer.size(), 0); // last parameter: flag. for us will be 0.
		}


		closesocket(clientSocket);
	}
	catch (const std::exception& e)
	{
		delete(m_clients[clientSocket]);
		m_clients.erase(clientSocket);
		closesocket(clientSocket);
	}
}
