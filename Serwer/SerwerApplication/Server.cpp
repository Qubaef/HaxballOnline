#include "stdafx.h"

Server::Server()
{
	this->pManager = new TransferManager();
	this->pGame = NULL;
}

Server::~Server()
{
	delete pGame;
	delete pManager;
}

// initialize server's components and start waiting for players to join
void Server::start()
{
	// initialize thread to accept connections from users on separate socket
	thread connectionsManager(&Server::manageConnections, this);

	printf_s("SERVER: Waiting for the game to start!\n");

	// main server loop awaiting for players to be ready to start the game
	while (true)
	{
		// check if there is enough players on the server
		// and if all of them confirmed that they are ready to play
		if (pManager->readyToPlay())
		{
			printf_s("SERVER: All players ready! Initializing game and waiting for their game to load!\n");
			play();
		}
	}
}

void Server::play()
{
	// initialize new game object
	pGame = new GameEngine();

	// for each client, create new player and add it to the game
	vector<ClientData*>* clientsData = this->pManager->getClientsData();
	for (ClientData* client : *clientsData)
	{
		client->setPlayer(new Player(pGame, 0, 0, client->getNumber()));
		pGame->newPlayer(client->getPlayer());
	}

	// build initialization pack and put it to dataToSendContainer
	pManager->buildInitializationPack();

	// reset readyToPlayFlags
	pManager->readyToPlayReset();

	// wait for all players to load their games
	while (!pManager->readyToPlay())
	{
	}

	// delete init pack
	pManager->deleteInitializationPack();

	printf_s("SERVER: All players ready! Game Starts!\n");
	pManager->setGameRunning(true);

	// main game loop
	while (pGame->getFinished() == false)
	{
		pGame->redraw();
		pManager->gameSerialize(pGame);
		for (ClientData* clientData : *(this->pManager->getClientsData()))
		{
			this->pManager->manageInputs(clientData);
		}
	}
	// out of the loop means that pGame -> getFinished() == true
	this->pManager->setGameRunning(false);

	// reset flags
	pManager->readyToPlayReset();
	pManager->dataToSendReset();

	delete pGame;
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
	if (iResult != 0)
	{
		printf_s("SERVER: WSAStartup failed with error: %d\n", iResult);
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf_s("SERVER: getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf_s("SERVER: socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	// Setup the TCP listening socket
	iResult = ::bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf_s("SERVER: bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf_s("SERVER: listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	// connections accept loop
	while (true)
	{
		printf_s("SERVER: Waiting for connections!\n");

		SOCKET ClientSocket = INVALID_SOCKET;

		// Accept a client socket
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET)
		{
			printf_s("SERVER: Accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return;
		}

		printf_s("SERVER: New client connected!\n");
		pManager->newClient(ClientSocket);
	}
}