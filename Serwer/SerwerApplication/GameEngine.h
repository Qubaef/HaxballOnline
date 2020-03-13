#pragma once
#include "stdafx.h"
class GameEngine
{
private:
	Goal* pGoals;
	Ball* pBall;
	vector<Player> players;
	vector<CirclePhysical> objects;
public:
	GameEngine();
	void newPlayer();
	void newBall();
	void update();
	void wallsCollsion(CirclePhysical* object);
	vector<CirclePhysical> getObjects();
	vector<CirclePhysical> getPlayers();
	Ball* getBall();
	Goal* getGoal();


};

