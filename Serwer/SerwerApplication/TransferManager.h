#pragma once
#include "stdafx.h"

class TransferManager
{
private:
	vector<ClientData*> clientsData;
	vector<thread> clientsThreads;
	void communicate(ClientData* data, unsigned int threadNumber);

	vector<bool> newDataToSend;			// array of flags set to True, if dataContainer contains new data ready to send to clients
	void* dataContainer;				// pointer to an array containing data, which needs to be sent to the clients
	unsigned int dataContainerLength;	// length of data in dataContainer

	GameEngine* pGame = NULL;
public:
	TransferManager();
	~TransferManager();
	void newClient(SOCKET clientSocket);
	bool readyToPlay();
	void sendInitializationPack();		// send info to all client threads, that they need to send initialization pack to their clients
	void* getInitializationPack();		// as a client thread, get pointer to a dataContainer which data needs to be sent (returns NULL if no data requires sending)
	vector<ClientData*>* getClientsData();
	void addGame(GameEngine* pGame);    //add game, throws exception if game already exist
};