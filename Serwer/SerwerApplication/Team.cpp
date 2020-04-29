#include "stdafx.h"

Team::Team(GameEngine* game, Goal* goal, short int pitchHalf)
{
	this->pGame = game;
	this->pGoal = goal;
	this->pitchHalf = pitchHalf;
	this->score = 0;
}


Team::~Team() {}


void Team::addPlayer(Player* pPlayer)
{
	this->players.push_back(pPlayer);
	pPlayer->setTeam(this->pitchHalf);
}


void Team::addPoint(){
	this->score += 1;
}


void Team::removePlayer(Player *pPlayer)
{
	for (int i = 0; i < players.size(); i++)
		if (players[i] == pPlayer)
			players.erase(players.begin() + i);	
}


void Team::resetPosition()
{
	int i = 1;
	for (Player* player : players)
	{
		double px = static_cast<double>(player->getGame()->getScreenW()) / 2 + static_cast<double>(this->pitchHalf * this->pGame->getPitchW() / 4);
		double py = static_cast<double>(player->getGame()->getScreenH() - player->getGame()->getPitchH()) / 2 + static_cast<double>(i * player->getGame()->getPitchH() / 4);
		player->setMove(Vector2D(0, 0));
		player->setPosition(Vector2D(px, py));
		i++;
	}
}


int Team::getScore(){
	return this->score;
}


void Team::resetScore(){
	this->score = 0;
}


int Team::size(){
	return static_cast<int>(players.size());
}


vector<Player*> Team::getPlayers(){
	return this->players;
}


void Team::serialize(vector<double> &dataVector) const
{
	dataVector.push_back(score);
	for (Player*player : players)
	{
		player->serialize(dataVector);
	}
}