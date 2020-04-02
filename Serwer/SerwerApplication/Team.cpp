#include "stdafx.h"

Team::Team(GameEngine* game, Goal* goal, int pitchHalf)
{
	this->pGame = game;
	this->pGoal = goal;
	this->pitchHalf = pitchHalf;
	this->score = 0;
}

Team::~Team()
{

}

void Team::addPlayer(Player* pPlayer)
{
	this->players.push_back(pPlayer);
}

void Team::addPoint()
{
	this->score += 1;
}

void Team::removePlayer(Player *pPlayer)
{
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i] == pPlayer)
		{
			players.erase(players.begin() + i);
		}
	}
}

void Team::resetPosition()
{
	int i = 1;
	for (Player* player : players)
	{
		float px = player->getGame()->getScreenW() / 2 + this->pitchHalf * this->pGame->getPitchW() / 4;
		float py = (player->getGame()->getScreenH() - player->getGame()->getPitchH()) / 2 + i * player->getGame()->getPitchH() / 4;
		player->setMove(Vector2D(0, 0));
		player->setPosition(Vector2D(px, py));
		i++;
	}
}

void Team::resetScore()
{
	this->score = 0;
}

int Team::size()
{
	return players.size();
}

vector<Player*> Team::getPlayers()
{
	return this->players;
}


double* Team::serialize()
{
	double* data = new double[TEAM_SIZE + players.size() * CIRCLE_SIZE];
	data[0] = score;
	int i = 0;
	for(Player*player:players)
	{
		double* dataPlayer = player->serialize();
		memcpy(&data[TEAM_SIZE + i * CIRCLE_SIZE], dataPlayer, CIRCLE_SIZE* sizeof(double));
		i++;
		delete dataPlayer;
	}
	return data;
}
