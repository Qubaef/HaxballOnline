
/**
 *  The following class describes a team ( a set of players ).
 */

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
	Team(GameEngine* game, Goal* goal, short int pitchHalf);
	~Team();

	void addPlayer(Player* player);		// adds player to a list of players
	void removePlayer(Player* player);	// removes player if necessary
	void resetScore();					// resets score when starting new game
	void addPoint();					// adds point when goal-event occured
	int size();							// returns size of team
	void resetPosition();				// resets players position to defaulty
	int getScore();						// gets score's value
	void serialize(vector<double>& dataVector) const;	// serializes team's data in case to be sent to the player

	vector<Player*> getPlayers();		// get list of all players
};