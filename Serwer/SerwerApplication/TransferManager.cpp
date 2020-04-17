#include "stdafx.h"

TransferManager::TransferManager()
{
	this->ifGameRunning = false;
}


// (New Thread) add new Client and start communicating with him
void TransferManager::newClient(SOCKET clientSocket)
{
	// create new client's data
	ClientData* newClient = new ClientData(clientSocket);

	// insert this data to vector in transfer manager
	ifdataToSend.push_back(false);
	clientsData.push_back(newClient);
	unsigned int threadNumber = clientsData.size() - 1;

	// call new thread to communicate with given client
	this->clientsThreads.push_back(thread(&TransferManager::communicate, this, newClient, threadNumber));
}


TransferManager::~TransferManager()
{
}


// function called bu separate thread
// communicate with your client
void TransferManager::communicate(ClientData* data, unsigned int threadIndex)
{
	vector<double> dataToSend;
	char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
	int iSendResult;
	int iResult;
	int playerSize;

	unsigned long l;
	ioctlsocket(data->getSocket(), FIONREAD, &l);

	// Get timestamp to track user's timeout
	std::chrono::time_point<std::chrono::system_clock> current = std::chrono::system_clock::now();
	printf_s("Communicating with client!\n");


	// CLIENT INITIALIZATION
	// ** Get client's nick
	iResult = customRecv(data, recvbuf);
	data->setNickname(bufferToString(recvbuf, iResult));
	printf_s("Received player's nick: %s\n", data->getNickname().c_str());


	// ** Generate client's number and send it to his socket
	// dataToSend.push_back(0);
	// dataToSend.push_back(0);
	dataToSend.push_back(generateNewNumber());
	iSendResult = send(data->getSocket(), (char*)&dataToSend[0], dataToSend.size() * sizeof(double), 0);


	while (true)
	{
		while (true)
		{
			// PRE-GAME COMMUNICATION PHASE
			// ** Receive "ready" flag from client
			iResult = customRecv(data, recvbuf);

			if (iResult != 1)
			{
				printf_s("Wrong data received from client: %s! Expected readyToPlay flag!", data->getNickname().c_str());
			}
			else
			{
				// Save ready flag state in client's data and resend it back to the user
				data->setReady(charToBool(recvbuf[0]));
				iSendResult = send(data->getSocket(), recvbuf, iResult, 0);
			}

			// ** Check if there is initialization pack ready to be sent
			// If true break the loop and send initialization pack
			if (this->ifdataToSend[threadIndex] == true)
			{
				break;
			}
		}


		// GAME PREPARATION PHASE
		// ** Send initialization pack to the client
		//we have to send first number of players and after it pack od initialization data
		// BUG?: convert initialization pack to array of chars and send it to the client
		// init pack will be accessable in saved in this->dataToSendContainer
		struct BasicInformation
		{
			int playerSize;
			int length;
		};

		//send basic info size of player and total length of initialize pack
		BasicInformation info;
		info.playerSize = this->clientsData.size();
		info.length = dataContainerLength;
		memcpy_s(sendbuf, sizeof(BasicInformation), &info, sizeof(info));
		iSendResult = send(data->getSocket(), sendbuf, sizeof(int), 0);

		//build and send initialization pack
		buildInitializationPack();
		iSendResult = send(data->getSocket(), static_cast<const char*>(this->dataToSendContainer), dataContainerLength, 0);

		while (true)
		{
			// GAME LOADING PHASE
			// ** Receive "ready" flag from client (if true, client has loaded the game and is ready to start the game)
			iResult = customRecv(data, recvbuf);

			if (iResult != 1)
			{
				printf_s("Wrong data received from client: %s! Expected readyGameLoaded flag!", data->getNickname().c_str());
			}
			else
			{
				// Save ready flag state in client's data and resend it back to the user
				data->setReady(charToBool(recvbuf[0]));
				iSendResult = send(data->getSocket(), recvbuf, iResult, 0);
			}

			if (this->ifGameRunning == true)
			{
				break;
			}
		}


		while (true)
		{
			// GAME RUNNING PHASE
			// ** Receive pack with user's input 
			iResult = customRecv(data, recvbuf);

			// ** Get data from dataToSendContainer and send it to the user

			// TODO: vector data from dataToSendContainer and send it to client

			// dataToSend = *((vector<double>*) dataToSendContainer);
			// memcpy(sendbuf, &dataToSend[0], dataToSend.size() * sizeof(double));
			// iSendResult = send(data->getSocket(), sendbuf, dataToSend.size() * sizeof(double), 0);

			// ** Analyze user's input and save it in client's data

			// TODO: save user's input (from recvbuf) in Client's data pack (ClientData* data)
			// TODO: in server.cpp, analyze user's input every frame and modify the game

			if (this->ifGameRunning == FALSE)
			{
				break;
			}
		}
	}

	closesocket(data->getSocket());
	WSACleanup();
}

// convert given char (byte) to bool value
bool TransferManager::charToBool(char flag)
{
	return flag == 0 ? false : true;
}


// convert char buffer to string obj
string TransferManager::bufferToString(char buffer[], int length)
{
	string str = "";
	for (int i = 0; i < length; i++)
	{
		str = str + buffer[i];
	}
	return str;
}


// generate and return new number for the player
unsigned short TransferManager::generateNewNumber()
{
	unsigned short max = 0;
	for (ClientData* client : this->clientsData)
	{
		max = client->getNumber() > max ? client->getNumber() : max;
	}
	return max + 1;
}


// check if every player is ready to play and if there is at least one player
bool TransferManager::readyToPlay()
{
	// check number of connected players
	if (this->clientsData.empty())
	{
		return false;
	}

	// check if all players are ready
	for (ClientData* client : this->clientsData)
	{
		if (client->getReady() == false)
		{
			return false;
		}
	}

	return true;
}


// get pointer to client's data vector
vector<ClientData*>* TransferManager::getClientsData()
{
	return &(this->clientsData);
}


void TransferManager::buildInitializationPack()
{
	// prepare initialization pack and place it in dataContainer
	PlayerInitializePack* initData = new PlayerInitializePack[this->clientsData.size()];
	unsigned int length = 0;

	for (int i = 0; i < this->clientsData.size(); i++)
	{
		initData[i].playerNickname = clientsData[i]->getNickname().c_str();
		initData[i].playerNumber = clientsData[i]->getNumber();
		initData[i].playerTeam = clientsData[i]->getPlayer()->getTeam();

		length += clientsData[i]->getNickname().length() + sizeof(initData[i].playerNumber) + sizeof(initData[i].playerTeam);
	}

	int playerSize = this->clientsData.size();
	this->dataToSendContainer = initData;
	this->dataContainerLength = length;

	// set all ifdataToSend flags to True
	for (bool sendFlag : this->ifdataToSend)
	{
		sendFlag = true;
	}
}


void TransferManager::dataSent(int threadNumber)
{
	this->ifdataToSend[threadNumber] = false;
}


// Function to execute player moves stored in their ClientsData structures
void TransferManager::manageInputs(GameEngine* pGame)
{
	// TODO
}


// Serialize data and put it into dataToSendContainer, setting ifdataToSend flags to true for every thread
void TransferManager::gameSerialize(GameEngine* pGame)
{
	// TODO: Critial section (?)
	// 
	// BUG: error while deleting void* (needs to be fixed)
	delete dataToSendContainer;
	dataToSendContainer = &(pGame->serialize());

	// set all ifdataToSend flags to True
	for (bool sendFlag : this->ifdataToSend)
	{
		sendFlag = true;
	}
}


void TransferManager::readyToPlayReset()
{
	// check if all players are ready
	for (ClientData* client : this->clientsData)
	{
		client->setReady(false);
	}
}


int TransferManager::customRecv(ClientData* data, char* recvbuf)
{
	int iResult;
	std::chrono::time_point<std::chrono::system_clock> current = std::chrono::system_clock::now();

	while (true)
	{
		iResult = recv(data->getSocket(), recvbuf, DEFAULT_BUFLEN, 0);

		//if we have error
		if (iResult == SOCKET_ERROR)
		{
			printf_s("send failed with error: %d\n", WSAGetLastError());
			return 0;
		}
		//if we recieve nothing
		if (iResult == 0)
		{
			//1. check how long is he not responding
			std::chrono::time_point<std::chrono::system_clock> now;
			now = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsed_seconds = now - current;

			//2. if time is too long, timeouting player from game
			if (elapsed_seconds.count() > TIMEOUT)
			{
				printf_s("User timeout: %s!\n", data->getNickname().c_str());
				return 0;
			}
		}
		else
		{
			return iResult;
		}
	}
}


void TransferManager::disablePlayer(ClientData* data)
{
	data->getPlayer()->setPosition(Vector2D(-10, -10));
}