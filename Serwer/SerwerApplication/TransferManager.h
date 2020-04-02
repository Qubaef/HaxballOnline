#pragma once
#include "stdafx.h"

class TransferManager
{
private:
	vector<ClientData*> clientsData;
	vector<thread> clientsThreads;
	void communicate(ClientData* data);
	GameEngine* pGame = NULL;
public:
	TransferManager();
	~TransferManager();
	void newClient(SOCKET clientSocket);
	bool readyToPlay();
	vector<ClientData*>* getClientsData();
	//add game, throws exception if game already exist
	void addGame(GameEngine* pGame);
};