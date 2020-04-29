
/**
 *  The following class describes all of the moving elements in the game - players, posts, ball.
 *  It enables to calculate objects collisions and any changes in their movement and position.
 */

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

	void velocityAdd(Vector2D velocity);		//adding velocity to speed
	virtual void update();						//updating speed, position

	virtual void setMove(Vector2D v);			//setter of v (velocity)
	Vector2D getMove();							//getter of v (velocity)

	virtual void setPosition(Vector2D p);		//setter of p (position)
	Vector2D getPosition();						//getter of p (position)

	GameEngine* getGame();		//getter of game
	double getVMax();			//getter of vMax
	int getSize();				//getter of size
	double getHitbox();			//getter of hitbox
	double getFriction();		//getter of friction
	double getBallControl();	//getter of ballControl
	double getWeight();			//getter of weight
	int getNumber();			//getter of number

	void collide();				//checking if a collision happened and solving it

	void serialize(vector<double> &dataVector) const;		//serializing circle data to be sent to the client
};
#endif