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
	//constructor 
	Team(GameEngine* game, Goal* goal, int pitchHalf);
	~Team();
	//add player to a list of players
	void addPlayer(Player* player);
	//remove player if necessery
	void removePlayer(Player* player);
	//reset score when starting new game
	void resetScore();
	//add point when goal scorred event occured
	void addPoint();
	//return size of team
	int size();
	//reset players position to defauly
	void resetPosition();
	//get list of players
	vector<Player*> getPlayers();
	//serialize team data to be sent to player
	double* serialize();
};
