#include "stdafx.h"
GameEngine::GameEngine()
{
	//this->pGoalLeft = (this, );
	//this->pGoalRight = ();
	//this->teamLeft();
	//this->teamRight();
}

void GameEngine::update()
{
	
}

void GameEngine::newBall()
{
	
}

void GameEngine::newPlayer()
{
	
}

void GameEngine::wallsCollsion(CirclePhysical* object)
{
	
}

vector<CirclePhysical> GameEngine::getObjects()
{
	return this->objects;
}

vector<Player> GameEngine::getPlayers()
{
	return this->players;
}

Ball* GameEngine::getBall()
{
	return this->pBall;
}

Goal* GameEngine::getGoal()
{
	return this->pGoals;
}



