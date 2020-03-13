#pragma once
#include "stdafx.h"
#include <vector>
#ifndef GAMEENGINE_H
#define GAMEENGINE_H
class GameEngine;
class CirclePhysical
{
private:
	GameEngine* pGame;
	Vector2D p;
	Vector2D v;
	const int size;
	const float hitbox;
	const float vMax;
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
};
#endif
