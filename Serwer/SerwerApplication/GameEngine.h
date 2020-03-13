#pragma once
#include "stdafx.h"
class GameEngine
{
private:
	const int screenW = 1100;
	const int screenH = (int)(screenW / 1.57);

	const int pitchW = (int)(screenW * 0, 8);
	const int pitchH = (int)(pitchW / 1.57);

	Ball* pBall;
	vector<Player> players;
	vector<CirclePhysical> objects;
	Goal* pGoalLeft;
	Goal* pGoalRight;
	Team* teamRight;
	Team* teamLeft;
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
	const int getScreenW();
	const int getScreenH();
	const int getPitchW();
	const int getPitchH();
};

