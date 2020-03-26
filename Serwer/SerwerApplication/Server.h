#pragma once
#include "stdafx.h"

class Server
{
private:
	GameEngine* pGame;
	TransferManager* pManager;
	void play();
	void manageConnections();
public:
	Server();
	~Server();
	void start();
};