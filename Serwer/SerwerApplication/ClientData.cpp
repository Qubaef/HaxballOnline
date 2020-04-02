#include "stdafx.h"

ClientData::ClientData(SOCKET clientSocket)
{
	this->ready = true;
	this->pPlayer = nullptr;		// player will be created right before game starts
	this->clientSocket = clientSocket;
}

ClientData::~ClientData()
{

}


bool ClientData::getReady()
{
	return this->ready;
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