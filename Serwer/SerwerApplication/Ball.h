#pragma once
#include "stdafx.h"
class Ball : public CirclePhysical
{
public:
	// the constructor of the ball
	Ball(GameEngine*game,int px,int py);
	// the destructor
	~Ball();
};