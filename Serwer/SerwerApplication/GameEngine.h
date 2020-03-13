#pragma once
#include "stdafx.h"
class GameEngine
{
private:
	Post posts[4];
	Ball* ball;
	vector<Player> players;
	int timer = 0;
public:
	GameEngine();
	void newPlayer();
	void newBall();
	void update();
	void wallsCollsion();
};

