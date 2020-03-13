#include "stdafx.h"

void Team::addPlayer(Player player)
{
	this->players.push_back(player);
}

void Team::addPoint()
{
	this->score += 1;
}

void Team::removePlayer(Player *player)
{
	for(int i=0;i<players.size();i++)
	{
		if(&players[i]==player)
		{
			players.erase(players.begin() + i);
		}
	}
}

void Team::resetPosition()
{
	int i = 1;
	for(Player player : players)
	{
		int px = player.getGame()->getScreenW() / 2 + this->pitchHalf * this->pGame->getScreenW() / 4;
		int py = (player.getGame()->getScreenH() - player.getGame()->getPitchH()) / 2 + i * player.getGame()->getPitchH() / 4;
		player.setMove(Vector2D(0, 0));
		player.setPosition(Vector2D(px, py));
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





