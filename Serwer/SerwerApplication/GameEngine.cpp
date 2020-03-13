#include "stdafx.h"
GameEngine::GameEngine()
{
	int screenMargin = (screenH - pitchH) / 2;
	this->pGoalLeft = new Goal(this, (screenW - pitchW) / 2, screenMargin + pitchH * 6 / 16, screenMargin + pitchH * 10 / 16, 50, -1);
	this->pGoalRight = new Goal(this, (screenW - pitchW) / 2, screenMargin + pitchH * 6 / 16, screenMargin + pitchH * 10 / 16, 50, 0);

	this->pTeamLeft = new Team(this, pGoalLeft, 1);
	this->pTeamRight = new Team(this, pGoalLeft, -1);
}

void GameEngine::redraw()
{
	this->gameStateManager();

	int dt = clockTick();

	if (delayCounter != 0) {
		delayCounter -= dt;
	}
	if (delayCounter < 0) {
		delayCounter = 0;
	}

	if (playMode == 0) {
		update();
	}
}

void GameEngine::update()
{
	// update players
	for (int i = 0; i < players.size(); i++) {
		players[i]->update();
	}

	// update ball
	pBall->update();

	// collide players
	for (int i = 0; i < players.size(); i++) {
		players[i]->collide();
	}

	// collide goals
	pGoalLeft->goalCollide();
	pGoalRight->goalCollide();

	// collide ball
	pBall->collide();
}

int GameEngine::clockTick()
{
	// TODO: write own clock tick
	return 0;
}

void GameEngine::gameStateManager()
{
	if (playMode == -2) {
		playMode = 1;
		delayCounter = goalDelay;
	}
	else if (playMode == -1 && delayCounter == 0) {
		playMode = 1;
	}
	else if (playMode == 1) {
		positionsReset();
		pTeamLeft->resetPosition();
		pTeamRight->resetPosition();

		playMode = 2;
		delayCounter = startDelay;
	}
	else if (playMode == 2 && delayCounter == 0) {
		playMode = 0;
	}
}

void GameEngine::positionsReset()
{
	pBall->setMove(*(new Vector2D(0, 0)));
	pBall->setPosition(*(new Vector2D(0, 0)));
}

void GameEngine::goal_scored(Goal* pGoal)
{
	if (pGoal == pGoalLeft) {
		pTeamRight->addPoint();
	}
	else {
		pTeamLeft->addPoint();
	}

	playMode = -2;
}

vector<CirclePhysical*> GameEngine::getObjects()
{
	// convert players to vector<CirclePhysical*>
	vector<CirclePhysical*> objects(players.begin(), players.end());

	// add ball and posts
	objects.push_back(pBall);
	objects.push_back(pGoalLeft->getPostDown());
	objects.push_back(pGoalLeft->getPostUp());
	objects.push_back(pGoalRight->getPostDown());
	objects.push_back(pGoalRight->getPostUp());

	return objects;
}

void GameEngine::newBall(Ball* pBall)
{
	this->pBall = pBall;
}

void GameEngine::newPlayer(Player* pPlayer, int teamNumber = 0)
{
	players.push_back(pPlayer);

	if (teamNumber == 0) {
		if (pTeamLeft->size() >= pTeamRight->size()) {
			pTeamRight->addPlayer(pPlayer);
		}
		else {
			pTeamLeft->addPlayer(pPlayer);
		}
	}
	else {
		if (teamNumber == 1) {
			pTeamLeft->addPlayer(pPlayer);
		}
		else {
			pTeamRight->addPlayer(pPlayer);
		}
	}
}

void GameEngine::wallsCollsion(CirclePhysical* pObject)
{
	// TODO: write wallsCollision
}


vector<Player*> GameEngine::getPlayers()
{
	return this->players;
}

Ball* GameEngine::getBall()
{
	return this->pBall;
}

Goal* GameEngine::getGoalRight()
{
	return this->pGoalRight;
}

Goal* GameEngine::getGoalLeft()
{
	return this->pGoalRight;
}

int GameEngine::getScreenW()
{
	return this->screenW;
}

int GameEngine::getScreenH()
{
	return this->screenH;
}

int GameEngine::getPitchW()
{
	return this->pitchW;
}

int GameEngine::getPitchH()
{
	return this->pitchH;
}



