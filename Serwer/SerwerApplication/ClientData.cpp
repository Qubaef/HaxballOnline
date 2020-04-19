#include "stdafx.h"

ClientData::ClientData(SOCKET clientSocket)
{
	this->ready = false;
	this->pPlayer = nullptr;			// player will be created right before game starts
	this->clientSocket = clientSocket;
	this->number = 0;
	this->nickname = "";
	this->userInput.command = 0;
	this->userInput.mouseXPos = 0;
	this->userInput.mouseYPos = 0;
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

void ClientData::setNumber(int number)
{
	this->number = number;
}


string ClientData::getNickname()
{
	return this->nickname;
}

void ClientData::setNickname(string nickname)
{
	this->nickname = nickname;
}


UserInput ClientData::getUserInput() const
{
	return this->userInput;
}

void ClientData::setUserInput(char * input)
{
	this->userInput.command = input[0];
	memcpy_s(&this->userInput.mouseXPos,sizeof(this->userInput.mouseXPos), input + 1,sizeof(double));
	memcpy_s(&this->userInput.mouseYPos, sizeof(this->userInput.mouseYPos), input + 1 + sizeof(double), sizeof(double));
}
