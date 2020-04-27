#include "stdafx.h"

GameEngine::GameEngine()
{
	int screenMargin = (screenH - pitchH) / 2;
	this->pGoalLeft = new Goal(this, (screenW - pitchW) / 2, screenMargin + pitchH * 6 / 16, screenMargin + pitchH * 10 / 16, 50, -1);
	this->pGoalRight = new Goal(this, pitchW + (screenW - pitchW) / 2, screenMargin + pitchH * 6 / 16, screenMargin + pitchH * 10 / 16, 50, 0);

	this->pBall = new Ball(this, 300, 300);

	this->pTeamRight = new Team(this, pGoalLeft, 1);
	this->pTeamLeft = new Team(this, pGoalLeft, -1);

	this->timer = chrono::high_resolution_clock::now();

	this->finished = false;
}

GameEngine::~GameEngine()
{
	delete pTeamRight;
	delete pTeamLeft;
	delete pBall;
	delete pGoalLeft;
	delete pGoalRight;
}


void GameEngine::redraw()
{
	this->gameStateManager();

	double dt = clockTick();

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
	// calculate frame percentage
	framePercentage = getTick() * milisPerFrame;

	// update players
	for (int i = 0; i < players.size(); i++)
	{
		//handling disconnected player
		if (players[i]->getPosition().getX() <= -10 && players[i]->getPosition().getY() <= -10)
			continue;

		players[i]->update();
	}

	// update ball
	pBall->update();

	// collide players
	for (int i = 0; i < players.size(); i++)
	{
		// handling disconnected player
		if (players[i]->getPosition().getX() <= -10 && players[i]->getPosition().getY() <= -10)
			continue;

		players[i]->collide();
	}

	// collide goals
	pGoalLeft->goalCollide();
	pGoalRight->goalCollide();

	// collide ball
	pBall->collide();
}


double GameEngine::getFramePercentage() {
	return framePercentage;
}


// return time since last tick (in miliseconds)
double GameEngine::clockTick() {
	chrono::high_resolution_clock::time_point current = chrono::high_resolution_clock::now();
	double dt = chrono::duration_cast<chrono::nanoseconds>(current - this->timer).count() / 1000000.0;
	this->timer = current;
	return dt;
}


// returns time since the last tick (in miliseconds)
double GameEngine::getTick() {
	chrono::high_resolution_clock::time_point current = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::nanoseconds>(current - this->timer).count() / 1000000.0;
}


// returns elapsed time since the game starts (in seconds)
double GameEngine::countDurationTime() {
	chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::nanoseconds>(now - this->gameStart).count() / 1000000000.0;
}


void GameEngine::gameStateManager()
{
	if (playMode == -2)
	{
		playMode = 1;
		delayCounter = goalDelay;
	}
	else if (playMode == -1 && delayCounter == 0)
	{
		playMode = 1;
	}
	else if (playMode == 1)
	{
		positionsReset();
		pTeamLeft->resetPosition();
		pTeamRight->resetPosition();
		playMode = 2;
		delayCounter = startDelay;
	}
	else if (playMode == 2 && delayCounter == 0)
	{
		// All set - game starts
		playMode = 0;
		elapsedTime = 0;
		if (pTeamLeft->getScore() + pTeamRight->getScore() == 0) // first round
		{
			this->gameStart = chrono::high_resolution_clock::now();
		}

		printf_s("GAME: Game starts!\n");
		printf_s("GAME: %d minutes left\n", gameDurationLimit - elapsedTime);
	}
	else if (playMode == 0) //game is running
	{
		// Game over
		if (countDurationTime() >= gameDurationLimit*60)		// minutes * 60secs
		{
			elapsedTime++;
			printf_s("GAME: %d minutes left\n", gameDurationLimit - elapsedTime);
			this->gameStart = chrono::high_resolution_clock::now();
		}
		if (elapsedTime >= this->gameDurationLimit)
		{
			// set all variables and game states				
			this->finished = true;
			this->playMode = -2;
			printf_s("GAME: Time's up! Game over\n");
			this->winnersInfo();
		}
	}
}


void GameEngine::positionsReset()
{
	pBall->setMove(Vector2D(0, 0));
	pBall->setPosition(Vector2D(screenW / 2, screenH / 2));
}


void GameEngine::goalScored(Goal* pGoal)
{
	if (pGoal == pGoalLeft) {
		printf_s("GAME: Goal for team right!\n");
		pTeamRight->addPoint();
	}
	else {
		printf_s("GAME: Goal for team left!\n");
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

void GameEngine::newBall(Ball* pBall){
	this->pBall = pBall;
}

void GameEngine::newPlayer(Player* pPlayer, int teamNumber)
{
	players.push_back(pPlayer);

	// choose in which team the player will be 
	if (teamNumber == 0)
	{
		if (pTeamLeft->size() >= pTeamRight->size())
		{
			pTeamRight->addPlayer(pPlayer);
			pPlayer->setTeam(2);
		}
		else
		{
			pTeamLeft->addPlayer(pPlayer);
			pPlayer->setTeam(1);
		}
	}
	else {
		if (teamNumber == 1)
		{
			pTeamLeft->addPlayer(pPlayer);
			pPlayer->setTeam(1);
		}
		else if (teamNumber == 2)
		{
			pTeamRight->addPlayer(pPlayer);
			pPlayer->setTeam(2);
		}
	}
}


void GameEngine::wallsCollision(CirclePhysical* pObject)
{
	// Top Wall
	if (pObject->getPosition().getY() < static_cast<int>(pObject->getSize() + (this->screenH - this->pitchH) / 2))
	{
		pObject->setPosition(Vector2D(pObject->getPosition().getX(), (int)(pObject->getSize() + (this->screenH - this->pitchH) / 2)));
		pObject->setMove(Vector2D(pObject->getMove().getX(), pObject->getMove().getY()*(-(this->wallBounce))));
	}

	// Bottom Wall
	if (pObject->getPosition().getY() > (int)(this->pitchH - pObject->getSize() + (this->screenH - this->pitchH) / 2))
	{
		pObject->setPosition(Vector2D(pObject->getPosition().getX(), (int)(this->pitchH - pObject->getSize() + (this->screenH - this->pitchH) / 2)));
		pObject->setMove(Vector2D(pObject->getMove().getX(), pObject->getMove().getY()*(-(this->wallBounce))));
	}

	if (Player* player = dynamic_cast<Player*>(pObject))
	{
		// Left Wall
		if (pObject->getPosition().getX() < (int)(pObject->getSize() + (this->screenW - this->pitchW) / 2))
		{
			if (pObject->getPosition().getY() < this->pGoalLeft->getPostDown()->getPosition().getY() && pObject->getPosition().getY() > this->pGoalLeft->getPostUp()->getPosition().getY())
			{
				if (pObject->getPosition().getX() < this->pGoalLeft->getPx())
				{
					pObject->setPosition(Vector2D(this->pGoalLeft->getPx(), pObject->getPosition().getY()));
					pObject->setMove(Vector2D(0, 0));
				}
			}
			else
			{
				pObject->setPosition(Vector2D((int)(pObject->getSize() + (this->screenW - this->pitchW) / 2), pObject->getPosition().getY()));
				pObject->setMove(Vector2D(pObject->getMove().getX()*(-(this->wallBounce)), pObject->getMove().getY()));
			}
		}

		// Right Wall
		if (pObject->getPosition().getX() > (int)(this->pitchW - pObject->getSize() + (this->screenW - this->pitchW) / 2))
		{
			if (pObject->getPosition().getY() < this->pGoalRight->getPostDown()->getPosition().getY() && pObject->getPosition().getY() > this->pGoalRight->getPostUp()->getPosition().getY())
			{
				if (pObject->getPosition().getX() > this->pGoalRight->getPx())
				{
					pObject->setPosition(Vector2D(this->pGoalRight->getPx(), pObject->getPosition().getY()));
					pObject->setMove(Vector2D(0, 0));
				}
			}
			else
			{
				pObject->setPosition(Vector2D((int)(this->pitchW - pObject->getSize() + (this->screenW - this->pitchW) / 2), pObject->getPosition().getY()));
				pObject->setMove(Vector2D(pObject->getMove().getX()*(-(this->wallBounce)), pObject->getMove().getY()));
			}
		}
	}


	if (Ball* ball = dynamic_cast<Ball*>(pObject))
	{
		// Left Wall
		if (pObject->getPosition().getX() < (int)(pObject->getSize() + (this->screenW - this->pitchW) / 2))
		{
			if (pObject->getPosition().getY() < this->pGoalLeft->getPostDown()->getPosition().getY() && pObject->getPosition().getY() > this->pGoalLeft->getPostUp()->getPosition().getY())
			{
				if (pObject->getPosition().getX() < this->pGoalLeft->getPx() - pObject->getSize())
				{
					goalScored(this->pGoalLeft);
				}
			}
			else
			{
				pObject->setPosition(Vector2D((int)(pObject->getSize() + (this->screenW - this->pitchW) / 2), pObject->getPosition().getY()));
				pObject->setMove(Vector2D(pObject->getMove().getX()*(-(this->wallBounce)), pObject->getMove().getY()));
			}
		}

		// Right Wall
		if (pObject->getPosition().getX() > (int)(this->pitchW - pObject->getSize() + (this->screenW - this->pitchW) / 2))
		{
			if (pObject->getPosition().getY() < this->pGoalRight->getPostDown()->getPosition().getY() && pObject->getPosition().getY() > this->pGoalRight->getPostUp()->getPosition().getY())
			{
				if (pObject->getPosition().getX() > this->pGoalRight->getPx() + pObject->getSize())
				{
					goalScored(this->pGoalRight);
				}
			}
			else
			{
				pObject->setPosition(Vector2D((int)(this->pitchW - pObject->getSize() + (this->screenW - this->pitchW) / 2), pObject->getPosition().getY()));
				pObject->setMove(Vector2D(pObject->getMove().getX()*(-(this->wallBounce)), pObject->getMove().getY()));
			}
		}
	}
}

void GameEngine::winnersInfo()
{
	if (pTeamLeft->getScore() == pTeamRight->getScore()) {
		printf_s("GAME: The game ended with a draw! \n\n");
	}
	else if (pTeamLeft->getScore() > pTeamRight->getScore()) {
		printf_s("GAME: And the winner is The Left Team! \n\n");
	}
	else {
		printf_s("GAME: And the winner is The Right Team! \n\n ");
	}
}

vector<Player*> GameEngine::getPlayers(){
	return this->players;
}

Ball* GameEngine::getBall(){
	return this->pBall;
}

Goal* GameEngine::getGoalRight(){
	return this->pGoalRight;
}

Goal* GameEngine::getGoalLeft(){
	return this->pGoalRight;
}

int GameEngine::getScreenW(){
	return this->screenW;
}

int GameEngine::getScreenH(){
	return this->screenH;
}

int GameEngine::getPitchW(){
	return this->pitchW;
}

int GameEngine::getPitchH(){
	return this->pitchH;
}

bool GameEngine::getFinished(){
	return this->finished;
}

vector<double> GameEngine::serialize() const
{
	//we don't have to send const value like goals and screen, we have to send balls and teams
	// here is a vectore to store serialized data
	vector<double>dataVector;

	//1. ball serialization
	pBall->serialize(dataVector);

	//2. team left serialization
	pTeamLeft->serialize(dataVector);

	//3.team right serialization
	pTeamRight->serialize(dataVector);

	return dataVector;
}