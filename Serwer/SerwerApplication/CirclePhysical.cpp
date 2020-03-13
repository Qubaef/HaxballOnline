#include "stdafx.h"


CirclePhysical::CirclePhysical(GameEngine* game, int px, int py, int number, float weight, int size)
	:game(game),p(px,py),number(number),weight(weight),size(size),v(0,0),vMax(6/pow(weight,2/3)),hitbox(size*3/2)
{
	
}
