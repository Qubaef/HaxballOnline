#include "stdafx.h"

Server::Server()
{
	this->pManager = new TransferManager();
}

void Server::start()
{
	// initialize thread to accept connections from users on separate socket

	// main server loop awaiting for players to be ready to start the game
	while (true)
	{
		// check if there is enough players on the server
		// and if all of them confirmed that they are ready to play
		// if (pManager->readyToPlay()) {
		//		play();
		// }
	}
}

void Server::play()
{
	// initialize new game
	pGame = new GameEngine();

	// main game loop
	while (pGame->getFinished() == false)
	{
		pGame->redraw();
	}
}
