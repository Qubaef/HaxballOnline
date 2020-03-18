#pragma once
#include "stdafx.h"
class Team
{
private:
	GameEngine* pGame;
	Goal* pGoal;
	vector<Player*> players;
	int score;
	int pitchHalf;
public:
	Team(GameEngine* game, Goal* goal, int pitchHalf);
	~Team();
	void addPlayer(Player* player);
	void removePlayer(Player* player);
	void resetScore();
	void addPoint();
	int size();
	void resetPosition();
};
