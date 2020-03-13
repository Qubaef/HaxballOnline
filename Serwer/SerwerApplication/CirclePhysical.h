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
	int ballControl;
	float weight;
	int number;
public:
	CirclePhysical(GameEngine *game, int px, int py, int number,float weight, int size);
	void velocityAdd(Vector2D velocity);
	virtual void update();
	virtual void setMove(Vector2D v);
	virtual void setPosition(Vector2D p);
	void collide();
	float getVMax();
	GameEngine* getGame();
	int getSize();
	float getHitbox();
	float getFriction();
	int getBallControl();
	float getWeight();
	int getNumber();
	Vector2D getPosition();
	Vector2D getMove();
};
#endif
