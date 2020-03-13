#pragma once
#include "stdafx.h"
class GameEngine
{
private:
	const int screenW = 1100;
	const int screenH = screenW / 1.57;

	const int pitchW = screenW * 0.8;
	const int pitchH = pitchW / 1.5;

	// delay time in miliseconds
	const int startDelay = 2000;
	const int goalDelay = 2000;
	int delayCounter = 0;

	int playMode = 1;
	// play_mode flags states:
	// play_mode = 0 = > game running
	// play_mode = -2 = > game freezed, players and ball not set on the right positions, waiting time not initialized(set after goal score)
	// play_mode = -1 = > game freezed, players and ball not set on the right positions, waiting time initialized(set after goal score and -2 state)
	// play_mode = 1 = > game freezed, players and ball set on the right positions, waiting time not initialized(set at the beginning of the game and after - 1 state(after goal score cooldown))
	// play_mode = 2 = > game freezed, players and ball set on the right positions, waiting time initialized(set after 1 state; after time counter drops to 0, game starts)

	Ball* pBall;
	vector<Player*> players;
	Goal* pGoalLeft;
	Goal* pGoalRight;
	Team* pTeamRight;
	Team* pTeamLeft;

public:
	GameEngine();
	void newPlayer(Player* pPlayer, int teamNumber);
	void newBall(Ball* pBall);
	void redraw();
	void update();
	void gameStateManager();
	void positionsReset();
	void goal_scored(Goal* pGoal);
	int clockTick();
	void wallsCollsion(CirclePhysical* pObject);
	vector<CirclePhysical*> getObjects();
	vector<Player*> getPlayers();
	Ball* getBall();
	Goal* getGoalRight();
	Goal* getGoalLeft();

	int getScreenW();
	int getScreenH();
	int getPitchW();
	int getPitchH();
};