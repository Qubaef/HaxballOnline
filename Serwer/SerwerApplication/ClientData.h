#pragma once
#include "stdafx.h"

class ClientData
{

private:
	string nickname;
	Player* pPlayer;
	SOCKET clientSocket;
	bool ready;
	unsigned short number;
	UserInput userInput;
public:
	ClientData(SOCKET clientSocket);
	~ClientData();
	
	bool getReady();
	void setReady(bool ready);
	
	void setPlayer(Player* pPlayer);
	Player* getPlayer();
	
	SOCKET getSocket();
	
	int getNumber();
	
	string getNickname();
	void setNickname(string nickname);

	UserInput getUserInput() const;

	void setUserInput(char* input);
};
