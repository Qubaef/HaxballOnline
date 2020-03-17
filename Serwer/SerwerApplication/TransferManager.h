#pragma once
#include "stdafx.h"

class TransferManager
{
private:
	vector<ClientData*> clientsData;
	vector<thread> clientsCommunicators;
	void communicate(ClientData* data);
public:
	TransferManager();
	void newClient();
	bool readyToPlay();
	vector<ClientData*>* getClientsData();
};