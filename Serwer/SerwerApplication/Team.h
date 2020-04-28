#pragma once
#include "stdafx.h"
class Team
{
private:
	GameEngine* pGame;			// actual game 
	Goal* pGoal;				// team's goal
	vector<Player*> players;	// set of all players from this team
	int score;					// how many goals the managed to score
	short int pitchHalf;		// to which half of the pitch they belong
	
public:
	//constructor & destructor
	Team(GameEngine* game, Goal* goal, short int pitchHalf);
	~Team();
	//adds player to a list of players
	void addPlayer(Player* player);
	//removes player if necessary
	void removePlayer(Player* player);
	//resets score when starting new game
	void resetScore();
	//adds point when goal-event occured
	void addPoint();
	//returns size of team
	int size();
	//resets players position to defaulty
	void resetPosition();
	// gets score's value
	int getScore();
	//get list of all players
	vector<Player*> getPlayers();
	//serialize team data in case to be sent to the player
	void serialize(vector<double> &dataVector) const;
};
