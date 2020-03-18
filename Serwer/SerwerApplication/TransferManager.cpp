#include "stdafx.h"

TransferManager::TransferManager()
{
}

TransferManager::~TransferManager()
{
}

// function called bu separate thread
// communicate with your client
void TransferManager::communicate(ClientData* data)
{
	while (true)
	{
		cout << "communicating with client!" << endl;
		this_thread::sleep_for(chrono::milliseconds(1000));
	}
}

// add new Client and start communicating with him
void TransferManager::newClient()
{
	// create new client's data
	ClientData* newClient = new ClientData();

	// insert this data to vector in transfer manager
	clientsData.push_back(newClient);

	// call new thread to communicate with given client
	this->clientsCommunicators.push_back(thread(&TransferManager::communicate, this, newClient));
}

// check if every player is ready to play and if there is at least one player
bool TransferManager::readyToPlay()
{
	// check number of connected players
	if (this->clientsData.size() == 0)
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