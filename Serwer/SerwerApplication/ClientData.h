#pragma once
#include "stdafx.h"

class ClientData
{
private:
	string nickname;
	Player* pPlayer;
	int lastInput;
	bool ready;
	unsigned short number;
public:
	ClientData();
	~ClientData();
	bool getReady();
	void setPlayer(Player* pPlayer);
	Player* getPlayer();
	int getNumber();
};
