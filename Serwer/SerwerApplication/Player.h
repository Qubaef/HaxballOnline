#pragma once
#include "stdafx.h"
class Player:public CirclePhysical
{
private:
	short int team;
public:
	Player(GameEngine* game, int px, int py, int number);
	~Player();
	short int getTeam();
	void setTeam(short int team);
	void kick(Vector2D pos);
	void modeBallControl();
	void modeNormal();
};

