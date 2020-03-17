#include "stdafx.h"

Team::Team(GameEngine* game, Goal* goal, int pitchHalf)
{
	this->pGame = game;
	this->pGoal = goal;
	this->pitchHalf = pitchHalf;
	this->score = 0;
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