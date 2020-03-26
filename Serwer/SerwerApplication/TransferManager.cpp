#include "stdafx.h"

TransferManager::TransferManager()
{
}

// (New Thread) add new Client and start communicating with him
void TransferManager::newClient(SOCKET clientSocket)
{
	// create new client's data
	ClientData* newClient = new ClientData(clientSocket);

	// insert this data to vector in transfer manager
	clientsData.push_back(newClient);

	// call new thread to communicate with given client
	this->clientsThreads.push_back(thread(&TransferManager::communicate, this, newClient));
}

TransferManager::~TransferManager()
{
}

// function called bu separate thread
// communicate with your client
void TransferManager::communicate(ClientData* data)
{
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	char sendbuf[DEFAULT_BUFLEN] = { "Hello, server here!" };
	int iResult;

	cout << "communicating with client!" << endl;

	while (true)
	{
		iResult = recv(data->getSocket(), recvbuf, DEFAULT_BUFLEN, 0);
		if (iResult > 0) {
			cout << "Bytes received: " << iResult << endl;
			cout << recvbuf;

			iSendResult = send(data->getSocket(), sendbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				cout << "send failed with error: " << WSAGetLastError() << endl;
				closesocket(data->getSocket());
				WSACleanup();
				return;
			}
			cout << "Bytes sent: " << iSendResult << endl;
		}
		else if (iResult == 0)
			// TODO: break connection here (?)
			continue;
		else {
			cout << "recv failed with error: " << WSAGetLastError() << endl;
			closesocket(data->getSocket());
			WSACleanup();
			return;
		}
	}
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