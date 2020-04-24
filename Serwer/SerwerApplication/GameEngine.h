#pragma once
#include "stdafx.h"
class GameEngine
{
private:
	const int screenW = 1100;
	const int screenH = static_cast<const int>(screenW / 1.57);

	const int pitchW = static_cast<const int>(screenW * 0.8);
	const int pitchH = static_cast<const int>(pitchW / 1.57);

	const int milisPerFrame = 1000 / 100;

	const double wallBounce = 1.0;

	// delay time in miliseconds
	const int startDelay = 2000;
	const int goalDelay = 2000;
	const int gameDurationLimit = 5 * 60; // game duration = 5mins = 300s
	double delayCounter = 0;
	chrono::high_resolution_clock::time_point gameStart;	
	chrono::high_resolution_clock::time_point timer;
	double framePercentage;

	int playMode = 1;
	// play_mode flags states:
	// play_mode = 0 = > game running
	// play_mode = -2 = > game freezed, players and ball not set on the right position, waiting, time not initialized (set after goal score)
	// play_mode = -1 = > game freezed, players and ball not set on the right position, waiting, time initialized (set after goal score and -2 state)
	// play_mode = 1  = > game freezed, players and ball set on the right position, waiting, time not initialized (set at the beginning of the game and after - 1 state(after goal score cooldown))
	// play_mode = 2  = > game freezed, players and ball set on the right position, waiting, time initialized (set after 1 state; after time counter drops to 0, game starts)


	Ball* pBall;
	vector<Player*> players;
	Goal* pGoalLeft;
	Goal* pGoalRight;
	Team* pTeamRight;
	Team* pTeamLeft;

	bool finished;		// flag to inform server that the game is over

public:
	GameEngine();
	~GameEngine();
	//adds a new player to the game
	void newPlayer(Player* pPlayer, int teamNumber = 0);
	//adds ball during game initialization
	void newBall(Ball* pBall);
	//updates game status if necessary
	void redraw();
	//updates game status
	void update();
	//manages state of the game ( game froze/running switching )
	void gameStateManager();
	//resets position state to default
	void positionsReset();
	//manages goal-scored event
	void goalScored(Goal* pGoal);
	// time management
	double clockTick();
	double getTick();
	double countDurationTime();
	// returns current framePercentage
	double getFramePercentage();
	//manages walls colision effect and updates player position, speed after it
	void wallsCollision(CirclePhysical* pObject);
	// calculates the winner team
	void winnersInfo();

	vector<CirclePhysical*> getObjects();
	vector<Player*> getPlayers();
	Ball* getBall();
	Goal* getGoalRight();
	Goal* getGoalLeft();

	int getScreenW();
	int getScreenH();
	int getPitchW();
	int getPitchH();
	bool getFinished();
	//serialize game data
	vector<double> serialize() const;
};