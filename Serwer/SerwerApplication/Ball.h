#pragma once
#include "stdafx.h"
class Ball : public CirclePhysical
{
public:
	Ball(GameEngine*game,int px,int py);
	~Ball();
};

