#include "stdafx.h"

ClientData::ClientData()
{
	this->ready = true;	// TMP solution; later, user will choose if he is ready
	this->pPlayer = nullptr;
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


int ClientData::getNumber()
{
	return this->number;
}