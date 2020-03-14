#include "stdafx.h"

Goal::Goal(GameEngine* pGame, int x, int yUp, int yDown, int width, int direction)
{
	this->pGame = pGame;
	this->x = x;
	this->yUp = yUp;
	this->yDown = yDown;
	this->width = width;
	this->direction = direction;
	this->pPostUp = new Post(pGame,x,yUp);
	this->pPostDown = new Post(pGame, x, yDown);
}

int Goal::getPx()
{
	return this->x;
}

int Goal::getPy()
{
	return this->yUp;
}

int Goal::getWidth()
{
	return this->width;
}

int Goal::getHeight()
{
	return (this->yDown)-(this->yUp);
}

void Goal::goalCollide()
{
	this->pPostDown->collide();
	this->pPostUp->collide();
}

Post* Goal::getPostUp()
{
	return pPostUp;
}

Post* Goal::getPostDown()
{
	return pPostDown;
}