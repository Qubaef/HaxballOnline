#pragma once
#include "stdafx.h"
class Player:public CirclePhysical
{
public:
	Player(GameEngine* game, int px, int py, int number);
	~Player();
	void kick(Vector2D pos);
	void modeBallControl();
	void modeNormal();
};

