#include "stdafx.h"

TransferManager::TransferManager()
{
	this->ifGameRunning = false;
	this->dataContainerLength = 0;
	this->dataPackToSend = vector<double>();
	this->initPackToSend = NULL;
}


// (New Thread) add new Client and start communicating with him
void TransferManager::newClient(SOCKET clientSocket)
{
	// create new client's data
	ClientData* newClient = new ClientData(clientSocket);

	// insert this data to vector in transfer manager
	ifdataToSend.push_back(false);
	clientsData.push_back(newClient);
	unsigned int threadNumber = static_cast<unsigned int>(clientsData.size() - 1);

	// call new thread to communicate with given client
	this->clientsThreads.push_back(thread(&TransferManager::communicate, this, newClient, threadNumber));
}


TransferManager::~TransferManager()
{

}


// function called bu separate thread
// communicate with your client
void TransferManager::communicate(ClientData* pData, unsigned int threadIndex)
{
	vector<double> dataToSend;
	char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
	int iSendResult;
	int iResult;

	unsigned long l;
	ioctlsocket(pData->getSocket(), FIONREAD, &l);

	// Get timestamp to track user's timeout
	std::chrono::time_point<std::chrono::system_clock> current = std::chrono::system_clock::now();
	printf_s("Communicating with client!\n");


	// CLIENT INITIALIZATION
	// ** Get client's nick
	iResult = customRecv(pData, recvbuf);
	pData->setNickname(bufferToString(recvbuf, iResult));
	printf_s("Received player's nick: %s\n", pData->getNickname().c_str());


	// ** Generate client's number and send it to his socket
	// dataToSend.push_back(0);
	// dataToSend.push_back(0);
	dataToSend.push_back(generateNewNumber());
	iSendResult = static_cast<int>(send(pData->getSocket(), (char*)&dataToSend[0], static_cast<int>(dataToSend.size()) * sizeof(double), 0));


	while (true)
	{
		while (true)
		{
			// PRE-GAME COMMUNICATION PHASE
			// ** Receive "ready" flag from client
			iResult = customRecv(pData, recvbuf);

			if (iResult != 1)
			{
				printf_s("Wrong data received from client: %s! Expected readyToPlay flag!", pData->getNickname().c_str());
			}
			else
			{
				// Save ready flag state in client's data and resend it back to the user
				pData->setReady(charToBool(recvbuf[0]));
				iSendResult = static_cast<int>(send(pData->getSocket(), recvbuf, iResult, 0));
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
		// we have to send first number of players and after it pack od initialization data
		// BUG?: convert initialization pack to array of chars and send it to the client

		// send basic info size of player and total length of initialize pack
		BasicInformation info;
		info.playerSize = static_cast<int>(this->clientsData.size());
		info.length = dataContainerLength;
		memcpy_s(sendbuf, sizeof(BasicInformation), &info, sizeof(info));
		iSendResult = static_cast<int>(send(pData->getSocket(), sendbuf, sizeof(int), 0));

		// build and send initialization pack
		buildInitializationPack();
		iSendResult = send(pData->getSocket(), static_cast<const char*>(this->initPackToSend), dataContainerLength, 0);

		while (true)
		{
			// GAME LOADING PHASE
			// ** Receive "ready" flag from client (if true, client has loaded the game and is ready to start the game)
			iResult = customRecv(pData, recvbuf);

			if (iResult != 1)
			{
				printf_s("Wrong data received from client: %s! Expected readyGameLoaded flag!", pData->getNickname().c_str());
			}
			else
			{
				// Save ready flag state in client's data and resend it back to the user
				pData->setReady(charToBool(recvbuf[0]));
				iSendResult = send(pData->getSocket(), recvbuf, iResult, 0);
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
			iResult = customRecv(pData, recvbuf);

			// ** Get data from dataToSendContainer and send it to the user
			memcpy(sendbuf, &dataToSend[0], dataToSend.size() * sizeof(double));
			iSendResult = static_cast<int>(send(pData->getSocket(), sendbuf, static_cast<int>(dataToSend.size()) * sizeof(double), 0));

			// ** Analyze user's input and save it in client's data
			pData->setUserInput(recvbuf);

			if (this->ifGameRunning == FALSE)
			{
				break;
			}
		}
	}

	closesocket(pData->getSocket());
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

		length += static_cast<unsigned int>(clientsData[i]->getNickname().length() + sizeof(initData[i].playerNumber) + sizeof(initData[i].playerTeam));
	}

	int playerSize = static_cast<int>(this->clientsData.size());
	this->initPackToSend = initData;
	this->dataContainerLength = length;

	// set all ifdataToSend flags to True
	for (bool sendFlag : this->ifdataToSend)
	{
		sendFlag = true;
	}
}

void TransferManager::deleteInitializationPack()
{
	delete[] initPackToSend;
	initPackToSend = NULL;
}



void TransferManager::dataSent(int threadNumber)
{
	this->ifdataToSend[threadNumber] = false;
}


// Function to execute player moves stored in their ClientsData structures
void TransferManager::manageInputs(ClientData * pClientData)
{
	if(pClientData->getUserInput().command)
	{
		if (pClientData->getUserInput().command & BALL_CONTROL)
		{
		    if(pClientData->getPlayer()->getBallControl()==1.0)
		    {
				pClientData->getPlayer()->modeBallControl();
		    }
			else
			{
				pClientData->getPlayer()->modeNormal();
			}
		}
		else if (pClientData->getUserInput().command & KICK)
		{
			pClientData->getPlayer()->kick(Vector2D(pClientData->getUserInput().mouseXPos, pClientData->getUserInput().mouseYPos));
		}
		else if (pClientData->getUserInput().command & MOUSE_RIGHT)
		{
			pClientData->getPlayer()->setMove(pClientData->getPlayer()->getMove() + Vector2D(1, 0));
		}
		else if (pClientData->getUserInput().command & MOUSE_LEFT)
		{
			pClientData->getPlayer()->setMove(pClientData->getPlayer()->getMove() + Vector2D(-1, 0));
		}
		else if (pClientData->getUserInput().command & MOUSE_UP)
		{
			pClientData->getPlayer()->setMove(pClientData->getPlayer()->getMove() + Vector2D(0, -1));
		}
		else if (pClientData->getUserInput().command & MOUSE_DOWN)
		{
			pClientData->getPlayer()->setMove(pClientData->getPlayer()->getMove() + Vector2D(0, 1));
		}
	}
    
}


// Serialize data and put it into dataToSendContainer, setting ifdataToSend flags to true for every thread
void TransferManager::gameSerialize(GameEngine* pGame)
{
	serializationSemaphore.lock();
	dataPackToSend.clear();
	dataPackToSend = pGame->serialize();
	serializationSemaphore.unlock();
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


int TransferManager::customRecv(ClientData* pData, char* recvbuf)
{
	int iResult;
	std::chrono::time_point<std::chrono::system_clock> current = std::chrono::system_clock::now();

	while (true)
	{
		iResult = recv(pData->getSocket(), recvbuf, DEFAULT_BUFLEN, 0);

		//if we have error
		if (iResult == SOCKET_ERROR)
		{
			printf_s("send failed with error: %d\n", WSAGetLastError());
			disablePlayer(pData);
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
				printf_s("User timeout: %s!\n", pData->getNickname().c_str());
				disablePlayer(pData);
				return 0;
			}
		}
		else
		{
			return iResult;
		}
	}
}


void TransferManager::disablePlayer(ClientData* pData)
{
	pData->getPlayer()->setPosition(Vector2D(-10, -10));
}