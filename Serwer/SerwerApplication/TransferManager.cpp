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
	unsigned int threadNumber = clientsData.size();

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
	// TODO: (Kwiaciu) write separate function to close the connection if error appears (if iResult will return SOCKET_ERROR)
	// It was like this previously:
	//  if (iSendResult == SOCKET_ERROR) {
	//  	cout << "send failed with error: " << WSAGetLastError() << endl;
	//  	closesocket(data->getSocket());
	//  	WSACleanup();
	//  	return;
	//  }
	//  but it will be better to write it in separate function for code reuse purposes


	// TODO: (Kwiaciu) write separate function to determine if timeout appeared
	// This is the code used previously
	// //1. check how long he is no responding
	// std::chrono::time_point<std::chrono::system_clock> now;
	// now = std::chrono::system_clock::now();
	// std::chrono::duration<double> elapsed_seconds = now - current;
	// 
	// //2. if time is too long, timeouting player from game
	// if (elapsed_seconds.count() < TIMEOUT)
	// 	continue;


	// TODO: (Kwiaciu) write separate function to close connection and disable player
	// It will be used after error appears


	vector<double> dataToSend;
	char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN];
	int iSendResult;
	int iResult;

	// Get timestamp to track user's timeout
	std::chrono::time_point<std::chrono::system_clock> current = std::chrono::system_clock::now();
	printf_s("Communicating with client!\n");


	// CLIENT INITIALIZATION
	// ** Get client's nick
	iResult = recv(data->getSocket(), recvbuf, DEFAULT_BUFLEN, 0);
	data->setNickname(bufferToString(recvbuf, iResult));
	printf_s("Received player's nick: %s\n", data->getNickname().c_str());


	// ** Generate client's number and send it to his socket
	// BUG: possible error because I have not added two zeroes at the beginning of the dataToSend vector (I want to see if it is needed)
	dataToSend.push_back(generateNewNumber());
	iSendResult = send(data->getSocket(), (char*)&dataToSend[0], dataToSend.size() * sizeof(double), 0);


	while (true)
	{
		while (true) {
			// PRE-GAME COMMUNICATION PHASE
			// ** Receive "ready" flag from client
			iResult = recv(data->getSocket(), recvbuf, DEFAULT_BUFLEN, 0);

			if (iResult != 1)
			{
				printf_s("Wrong data received from client: %s! Expected readyToPlay flag!", data->getNickname().c_str());
			}
			else
			{
				// Save ready flag state in client's data and resend it back to the user
				data->setReady(recvbuf[0]);
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

		// TODO: (Kwiaciu) convert initialization pack to array of chars and send it to the client
		// init pack will be accessable in saved in this->dataToSendContainer


		while (true) {
			// GAME LOADING PHASE
			// ** Receive "ready" flag from client (if true, client has loaded the game and is ready to start the game)
			iResult = recv(data->getSocket(), recvbuf, DEFAULT_BUFLEN, 0);

			if (iResult != 1)
			{
				printf_s("Wrong data received from client: %s! Expected readyGameLoaded flag!", data->getNickname().c_str());
			}
			else
			{
				// Save ready flag state in client's data and resend it back to the user
				data->setReady(recvbuf[0]);
				iSendResult = send(data->getSocket(), recvbuf, iResult, 0);
			}

			if (this->ifGameRunning == true) {
				break;
			}
		}


		while (true) {
			// GAME RUNNING PHASE
			// ** Receive pack with user's input 
			iResult = recv(data->getSocket(), recvbuf, DEFAULT_BUFLEN, 0);

			// ** Get data from dataToSendContainer and send it to the user

			// TODO: vector data from dataToSendContainer and send it to client

			// dataToSend = *((vector<double>*) dataToSendContainer);
			// memcpy(sendbuf, &dataToSend[0], dataToSend.size() * sizeof(double));
			// iSendResult = send(data->getSocket(), sendbuf, dataToSend.size() * sizeof(double), 0);

			// ** Analyze user's input and save it in client's data

			// TODO: save user's input (from recvbuf) in Client's data pack (ClientData* data)
			// TODO: in server.cpp, analyze user's input every frame and modify the game

			if (this->ifGameRunning == FALSE) {
				break;
			}
		}
	}

	closesocket(data->getSocket());
	WSACleanup();
}


// convert char buffer to string obj
string TransferManager::bufferToString(char buffer[], int length)
{
	string str = "";
	for (int i = 0; i < length; i++) {
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

	for (ClientData* client : this->clientsData)
	{
		initData->playerNickname = client->getNickname();
		initData->playerNumber = client->getNumber();
		initData->playerTeam = client->getPlayer()->getTeam();

		length += sizeof(PlayerInitializePack);
	}

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