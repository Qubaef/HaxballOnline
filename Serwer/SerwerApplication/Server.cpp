#include "stdafx.h"
#include <minwindef.h>

Server::Server()
{
	this->pManager = new TransferManager();
}

Server::~Server()
{
	delete pGame;
	delete pManager;
}

void Server::start()
{
	// initialize thread to accept connections from users on separate socket
	thread connectionsManager(&Server::manageConnections, this);

	cout << "Waiting for game to play!" << endl;

	// main server loop awaiting for players to be ready to start the game
	while (true)
	{


		// check if there is enough players on the server
		// and if all of them confirmed that they are ready to play
		if (pManager->readyToPlay()) {
			cout << "All players ready! Starting..." << endl;
			play();
		}
	}
}

void Server::play()
{
	// initialize new game
	pGame = new GameEngine();
	pManager->addGame(pGame);
	// for each client, create new player and add it to the game
	vector<ClientData*>* clientsData = this->pManager->getClientsData();
	for (ClientData* client : *clientsData)
	{
		client->setPlayer(new Player(pGame, 0, 0, client->getNumber()));
		pGame->newPlayer(client->getPlayer());
	}

	// main game loop
	while (pGame->getFinished() == false)
	{
		pGame->redraw();
	}
}

// (New Thread) initialize sockets and manage clients connections
void Server::manageConnections()
{
	//------- create and initialize socket
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "WSAStartup failed with error: %d" << iResult << endl;
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		cout << "getaddrinfo failed with error: %d" << iResult << endl;
		WSACleanup();
		return;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		cout << "socket failed with error: %ld\n"<< WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	// Setup the TCP listening socket
	iResult = ::bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		cout << "bind failed with error: %d\n" << WSAGetLastError() << endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		cout << "listen failed with error: %d\n" << WSAGetLastError() << endl;
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	// connections accept loop
	while (true)
	{
		cout << "Managing connections!" << endl;

		SOCKET ClientSocket = INVALID_SOCKET;

		// Accept a client socket
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			cout << "accept failed with error: %d\n" << WSAGetLastError() << endl;
			closesocket(ListenSocket);
			WSACleanup();
			return;
		}

		cout << "New client connected!" << endl;
		pManager->newClient(ClientSocket);
	}
}