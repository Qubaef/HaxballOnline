#pragma once
#include "stdafx.h"
class Player:public CirclePhysical
{
public:
	Player(GameEngine* game, int px, int py, int number);
	void kick(Vector2D pos);
	void modeBallControl();
	void modeNormal();
};

