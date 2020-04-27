#pragma once
#include "stdafx.h"
#include <vector>
#ifndef GAMEENGINE_H
#define GAMEENGINE_H
class GameEngine;
class CirclePhysical
{
protected:
	GameEngine* pGame;		// current game
	Vector2D p;				// the vector of position
	Vector2D v;				// the vector of velocity
	const int size;	
	const double hitbox;	// area around each object
	double vMax;			// maximum value of velocity
	double friction;	
	double ballControl;	
	double weight;	
	int number;	
public:
	CirclePhysical(GameEngine *game, int px, int py, int number, double weight, int size);
	~CirclePhysical();

	//adding velocity to speed
	void velocityAdd(Vector2D velocity);

	//update speed, position
	virtual void update();

	//setter of v (velocity)
	virtual void setMove(Vector2D v);

	//setter of p (position)
	virtual void setPosition(Vector2D p);

	//check if collision happened and solve it
	void collide();

	//getter of vMax
	double getVMax();

	//getter of game
	GameEngine* getGame();

	//getter of size
	int getSize();

	//getter of hitbox
	double getHitbox();

	//getter of friction
	double getFriction();

	//getter of ballControl
	double getBallControl();

	//getter of weight
	double getWeight();

	//getter of number
	int getNumber();

	//getter of p (position)
	Vector2D getPosition();

	//getter of v (velocity)
	Vector2D getMove();

	//serialize circle data to be send to the client
	void serialize(vector<double> &dataVector) const;
};
#endif
