#pragma once
#include "stdafx.h"

class Server
{
private:
	GameEngine* pGame;
	TransferManager* pManager;
	void play();
public:
	Server();
	void start();
};