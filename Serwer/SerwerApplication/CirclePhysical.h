#pragma once
#include "stdafx.h"
#include <vector>
#ifndef GAMEENGINE_H
#define GAMEENGINE_H
class GameEngine;
class CirclePhysical
{
protected:
	GameEngine* pGame;
	Vector2D p;
	Vector2D v;
	const int size;
	const float hitbox;
	float vMax;
	float friction;
	float ballControl;
	float weight;
	int number;
public:
	CirclePhysical(GameEngine *game, int px, int py, int number, float weight, int size);
	~CirclePhysical();

	//adding velocity to speed
	void velocityAdd(Vector2D velocity);

	//update speed, position
	virtual void update();

	//setter of v
	virtual void setMove(Vector2D v);

	//setter of p
	virtual void setPosition(Vector2D p);

	//check if collision happened and solve it
	void collide();

	//getter of vMax
	float getVMax();

	//getter of game
	GameEngine* getGame();

	//getter of size
	int getSize();

	//getter of hitbox
	float getHitbox();

	//getter of friction
	float getFriction();

	//getter of ballControl
	float getBallControl();

	//getter of weight
	float getWeight();

	//getter of number
	int getNumber();

	//getter of p
	Vector2D getPosition();

	//getter of v
	Vector2D getMove();

	//serialize circle data to be send to the client
	double* serialize();
};
#endif
