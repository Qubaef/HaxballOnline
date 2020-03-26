#pragma once
#include "stdafx.h"

class TransferManager
{
private:
	vector<ClientData*> clientsData;
	vector<thread> clientsThreads;
	void communicate(ClientData* data);
public:
	TransferManager();
	~TransferManager();
	void newClient(SOCKET clientSocket);
	bool readyToPlay();
	vector<ClientData*>* getClientsData();
};