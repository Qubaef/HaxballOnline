#include "stdafx.h"

ClientData::ClientData(SOCKET clientSocket)
{
	this->ready = false;
	this->pPlayer = nullptr;			// player will be created right before game starts
	this->clientSocket = clientSocket;
	this->number = 0;
	this->nickname = "";
}

ClientData::~ClientData()
{
}


bool ClientData::getReady()
{
	return this->ready;
}


void ClientData::setReady(bool ready)
{
	this->ready = ready;
}



void ClientData::setPlayer(Player* pPlayer)
{
	this->pPlayer = pPlayer;
}

Player* ClientData::getPlayer()
{
	return this->pPlayer;
}

SOCKET ClientData::getSocket()
{
	return this->clientSocket;
}

int ClientData::getNumber()
{
	return this->number;
}

string ClientData::getNickname()
{
	return this->nickname;
}

void ClientData::setNickname(string nickname)
{
	this->nickname = nickname;
}
