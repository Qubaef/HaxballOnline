
/**
 *  The following class describes the posts ( goal components ).
 *  This class is a derived class and inherits from the "CirclePhysical" base class.
 */

#pragma once
#include "stdafx.h"

class Post:public CirclePhysical
{
	// Each of 4 posts implements function from an above class (CirclePhysical). As far as we are concerned,
	// posts are static and don't move, so we only have to set the initial values of their position and 
	// leave the rest of the functions blank.
public:
	Post(GameEngine* game, int px, int py);
	~Post();
	void update() override;
	void setPosition(Vector2D p) override;
	void setMove(Vector2D v)override;
};