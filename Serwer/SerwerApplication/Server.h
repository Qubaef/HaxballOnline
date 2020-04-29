#pragma once
#include "stdafx.h"

class Server
{
private:
	GameEngine* pGame;			// current game
	TransferManager* pManager;	// necessary manager to the current game
	void play();				// starting the actual game ( main loop )
	void manageConnections();	// managing connections with all clients 

public:
	Server();
	~Server();

	// initializing server's components and start waiting for players to join
	void start();				

};