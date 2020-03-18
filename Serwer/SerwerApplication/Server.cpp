#include "stdafx.h"

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

	// main server loop awaiting for players to be ready to start the game
	while (true)
	{
		cout << "Waiting for game to play!" << endl;
		this_thread::sleep_for(chrono::milliseconds(1000));
		// check if there is enough players on the server
		// and if all of them confirmed that they are ready to play
		if (pManager->readyToPlay()) {
			play();
		}
	}
}

void Server::play()
{
	// initialize new game
	pGame = new GameEngine();

	// for each client, create new player and add it to the game
	vector<ClientData*>* clientsData = this->pManager->getClientsData();
	for(ClientData* client : *clientsData)
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


void Server::manageConnections()
{
	// simulate connection of new clients
	this->pManager->newClient();
	this->pManager->newClient();
	this->pManager->newClient();
	this->pManager->newClient();

	while (true)
	{
		cout << "Managing connections!" << endl;
		this_thread::sleep_for(chrono::milliseconds(1000));
	}
}