
/**
 *  The following class stores data about the current game. 
 *  It also makes available many functions to manage and coordinate the game.
 */

#pragma once
#include "stdafx.h"

class GameEngine
{
private:		// Game parameters below
	const int screenW = 1100;
	const int screenH = static_cast<const int>(screenW / 1.57);

	// pitch is obviously a little bit smaller than the whole game window
	const int pitchW = static_cast<const int>(screenW * 0.8);
	const int pitchH = static_cast<const int>(pitchW / 1.57);

	const int milisPerFrame = 1000 / 100;			// refreshing frequency 
	const double wallBounce = 1.0;					// the coefficient of elasticity

	// delays of time in milliseconds
	const int startDelay = 2000;
	const int goalDelay = 2000;
	double delayCounter = 0;

	const unsigned gameDurationLimit = 1;		// game duration (in minutes)
	unsigned elapsedTime;						// elapsed time (in minutes)

	chrono::high_resolution_clock::time_point gameStart;	// since the particular game starts 
	chrono::high_resolution_clock::time_point timer;		// since the server starts

	double framePercentage;

	int playMode = 1;
	// play_mode flags states:
	// play_mode = 0 = > game running
	// play_mode = -2 = > game freezed, players and ball not set on the right position, waiting, time not initialized (set after goal score)
	// play_mode = -1 = > game freezed, players and ball not set on the right position, waiting, time initialized (set after goal score and -2 state)
	// play_mode = 1  = > game freezed, players and ball set on the right position, waiting, time not initialized (set at the beginning of the game and after - 1 state(after goal score cooldown))
	// play_mode = 2  = > game freezed, players and ball set on the right position, waiting, time initialized (set after 1 state; after time counter drops to 0, game starts)

	Ball* pBall;				// pointer to the ball from the game 
	vector<Player*> players;	// set of all players
	Goal* pGoalLeft;			// the left team's goal	
	Goal* pGoalRight;			// the right team's goal
	Team* pTeamLeft;			// the left team
	Team* pTeamRight;			// the right team

	bool finished;				// flag to inform server that the game is over

public:
	GameEngine();
	~GameEngine();

	void newPlayer(Player* pPlayer, int teamNumber = 0);		//adds a new player to the game
	void newBall(Ball* pBall);		//adds ball during game initialization
	void redraw();					//updates game status if necessary
	void update();					//updates game status
	void gameStateManager();		//manages state of the game ( game froze/running switching )
	void positionsReset();			//resets position state to default		
	void goalScored(Goal* pGoal);	//manages goal-scored event

	// time management
	double clockTick();
	double getTick();
	double countDurationTime();
	double getFramePercentage();			// returns current framePercentage

	//manages walls collision effect and updates player position, speed after it
	void wallsCollision(CirclePhysical* pObject);
	// calculates the winner team
	void winnersInfo();

	vector<CirclePhysical*> getObjects();		// getting round, material all objects (ball+posts)
	vector<Player*> getPlayers();				// getting all players from the pitch 
	Ball* getBall();							// getting the ball
	Goal* getGoalRight();						// getting goal from the rigth side of the pitch
	Goal* getGoalLeft();						// and from the left side as well

	// getters to the other private variables from this class
	int getScreenW();
	int getScreenH();
	int getPitchW();
	int getPitchH();
	bool getFinished();

	vector<double> serialize() const;		//getter to the serialized game's data
};