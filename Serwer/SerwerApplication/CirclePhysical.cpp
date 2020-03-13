#include "stdafx.h"


CirclePhysical::CirclePhysical(GameEngine* game, int px, int py, int number, float weight, int size)
	:pGame(game),p(px,py),number(number),weight(weight),size(size),v(0,0),vMax(6/pow(weight,2/3)),hitbox(size*3/2),ballControl(1)
{
	
}
void CirclePhysical::setPosition(Vector2D p)
{
	this->p = p;
}

void CirclePhysical::setMove(Vector2D v)
{
	this->v = v;
}

void CirclePhysical::update()
{
	this->p = this->p + this->v;
}

void CirclePhysical::collide()
{
	vector<CirclePhysical> objects = this->pGame->getObjects();
	for(CirclePhysical object:objects)
	{
		if(&object!=this)
		{
			float dist = sqrt(pow((this->p.getX() - object.p.getX()), 2) + pow((this->p.getY() - object.p.getY()), 2));
			if(dist<=object.size + this->size)
			{
				float overlap = (dist - this->size - object.size);
				
			}
		}
	}
}

void CirclePhysical::velocityAdd(Vector2D velocity)
{
	
}




