
/**
 *  The following class describes the ball used while playing game.
 *  This class is a derived class and inherits from the "CirclePhysical" base class.
 */

#pragma once
#include "stdafx.h"

class Ball : public CirclePhysical 
{
public:
	Ball(GameEngine*game, int px, int py);
	~Ball();
};