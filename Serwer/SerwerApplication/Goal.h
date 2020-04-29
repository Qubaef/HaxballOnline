
/**
 *  The following class describes goals set on the opposite sides of the pitch.
 *	Each goal contains two posts ( the upper and the lower one ).
 */

#pragma once
#include "stdafx.h"

class Goal
{
private:
	GameEngine* pGame;		// to which game the goal is set to
	int x;					// the goal's position coordinates
	int yUp;				// --||--
	int yDown;				// --||--
	int width;				// the goal's width (measured in horizontal)
	int direction;			// direction in which the goal should be turned ( to face the pitch, of course )
	Post *pPostUp;			// pointers to the goal's posts ( up and down ) 
	Post *pPostDown;

public:
	Goal(GameEngine* pGame, int x, int yUp, int yDown, int width, int direction);
	~Goal();

	// getters for the private variables
	int getPx();
	int getPy();
	int getWidth();
	int getHeight();
	Post* getPostUp();
	Post* getPostDown();

	// calculating collisions with ball/players (uses "collide" function from CirclePhysical class)
	void goalCollide();
};