#pragma once
#include "stdafx.h"
class GameEngine
{
private:
	Goal* pGoals;
	Ball* pBall;
	vector<Player> players;
	vector<CirclePhysical> objects;
	int timer = 0;
public:
	GameEngine();
	void newPlayer();
	void newBall();
	void update();
	void wallsCollsion();
	vector<CirclePhysical> getObjects();
};

