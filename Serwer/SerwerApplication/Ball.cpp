#include "stdafx.h"
#include "Ball.h"


Ball::Ball(GameEngine* game, int px, int py)
	:CirclePhysical(game,px,py,0,0.2,10) {}

Ball::~Ball() {}