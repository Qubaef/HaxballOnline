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

	// Set socket's mode to non-blocking one
	unsigned long l;
	ioctlsocket(pData->getSocket(), FIONREAD, &l);

	printf_s("Communicating with client!\n");


	// CLIENT INITIALIZATION
	// Get client's nick
	iResult = customRecv(pData, recvbuf);
	if (iResult == 0)
	{
		closesocket(pData->getSocket());
		WSACleanup();
		return;
	}
	pData->setNickname(bufferToString(recvbuf, iResult));
	printf_s("Received player's nick: %s\n", pData->getNickname().c_str());


	// Generate client's number and send it to his socket
	pData->setNumber(generateNewNumber());
	dataToSend.push_back(pData->getNumber());
	iSendResult = static_cast<int>(send(pData->getSocket(), (char*)&dataToSend[0], static_cast<int>(dataToSend.size()) * sizeof(double), 0));


	while (true)
	{
		while (true)
		{
			// PRE-GAME COMMUNICATION PHASE
			// Receive "ready" flag from client
			iResult = customRecv(pData, recvbuf);
			if (iResult == 0)
			{
				closesocket(pData->getSocket());
				WSACleanup();
				return;
			}
			// Check if there is initialization pack ready to be sent
			// If true break the loop and send initialization pack
			if (this->ifdataToSend[threadIndex] == false)
			{
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
			}
			else
			{
				break;
			}
		}


		// GAME PREPARATION PHASE
		// ** Send initialization pack to the client

		// send number of players at the start
		int playersNumber = static_cast<int>(this->clientsData.size());
		memcpy_s(sendbuf, sizeof(int), &playersNumber, sizeof(int));
		iSendResult = static_cast<int>(send(pData->getSocket(), sendbuf, sizeof(int), 0));

		// receive number of players back as a confirmation
		iResult = customRecv(pData, recvbuf);


		// build and send initialization packs for each player
		for (unsigned int i = 0; i < dataContainerLength; i++)
		{
			int packageLength = static_cast<int>(initPackToSend[i].playerNickname.size()) * sizeof(char);

			// copy data to sendbuf in correct way
			memcpy_s(sendbuf, sizeof(int), &(initPackToSend[i].playerTeam), sizeof(int));
			memcpy_s(sendbuf + sizeof(int), sizeof(int), &(initPackToSend[i].playerNumber), sizeof(int));
			memcpy_s(sendbuf + sizeof(int) * 2, packageLength, &(initPackToSend[i].playerNickname[0]), packageLength);

			// send buffer with data
			iSendResult = send(pData->getSocket(), sendbuf, packageLength + 2 * sizeof(int), 0);
		}

		// set flag that init pack was sent
		this->ifdataToSend[threadIndex] = false;


		while (true)
		{
			// GAME LOADING PHASE
			// ** Receive "ready" flag from client (if true, client has loaded the game and is ready to start the game)
			iResult = customRecv(pData, recvbuf);
			if (iResult == 0)
			{
				closesocket(pData->getSocket());
				WSACleanup();
				return;
			}
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
			// ** Get data from dataToSendContainer and send it to the user
			if (this->ifdataToSend[threadIndex])
			{
				double longMessageFix = 0;
				memcpy_s(sendbuf, sizeof(double), &longMessageFix, sizeof(double));
				memcpy_s(sendbuf + sizeof(double), sizeof(double), &longMessageFix, sizeof(double));
				memcpy_s(sendbuf + 2 * sizeof(double), dataPackToSend.size() * sizeof(double), &dataPackToSend[0], dataPackToSend.size() * sizeof(double));
				// TODO: line below sends wrong data because semaphore does not work
				iSendResult = static_cast<int>(send(pData->getSocket(), sendbuf, static_cast<int>(dataPackToSend.size()) * sizeof(double), 0));
			}
			// ** Receive pack with user's input 
			iResult = customRecv(pData, recvbuf);
			if(iResult==0)
			{
				closesocket(pData->getSocket());
				WSACleanup();
				return;
			}
			// ** Analyze user's input and save it in client's data
			pData->setUserInput(recvbuf);

			if (this->ifGameRunning == FALSE)
			{
				break;
			}
		}
	}
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
		initData[i].playerNickname = clientsData[i]->getNickname();
		initData[i].playerNumber = clientsData[i]->getNumber();
		initData[i].playerTeam = clientsData[i]->getPlayer()->getTeam();

		length++;
	}

	this->initPackToSend = initData;
	this->dataContainerLength = length;

	// set all ifdataToSend flags to True
	for (int i = 0; i < ifdataToSend.size(); i++)
	{
		ifdataToSend[0] = true;
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
	if (pClientData->getUserInput().command)
	{
		if (pClientData->getUserInput().command & BALL_CONTROL)
		{
			if (pClientData->getPlayer()->getBallControl() == 1.0)
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
	dataPackToSendMutex.lock();

	// TODO: semaphore does not work
	dataPackToSend.clear();
	dataPackToSend = pGame->serialize();

	dataPackToSendMutex.unlock();

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
			printf_s("recieve failed with error: %d\n", WSAGetLastError());
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


void TransferManager::setGameRunning(bool ifGameRunning)
{
	this->ifGameRunning = ifGameRunning;
}
