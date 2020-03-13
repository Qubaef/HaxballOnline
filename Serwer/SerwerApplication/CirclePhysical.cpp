#include "stdafx.h"


CirclePhysical::CirclePhysical(GameEngine* game, int px, int py, int number, float weight, int size)
	:pGame(game),p(px,py),number(number),weight(weight),size(size),v(0,0),vMax(6/pow(weight,2/3)),hitbox(size*3/2),ballControl(1),friction(weight*0.2)
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
	this->v = this->v - this->v * friction;
	this->p = this->p + this->v;

	if(this->v.length()>this->getVMax())
	{
		this->setMove(Vector2D::Normal(this->v) * this->getVMax());
		this->pGame->wallsCollsion(this);
	}
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
				if(dist!=0)
				{
					Vector2D pNew = this->p - (this->p - object.p)* overlap * (object.weight / this->weight) / dist;
					this->setPosition(pNew);
				}
				else
				{
					Vector2D pNew = this->p - Vector2D::Normal(this->p) * overlap * (object.weight / this->weight);
					this->setPosition(pNew);
				}
				this->pGame->wallsCollsion(this);
				if(dist!=0)
				{
					Vector2D pNew = object.p - (object.p - this->p) * overlap * (this->weight / object.weight) / dist;
					object.setPosition(pNew);
				}
				object.pGame->wallsCollsion(&object);
				float mass = 2 * this->weight / (this->weight + object.weight);
				this->setMove(this->v - (this->p - object.p) * (mass * Vector2D::Dot(this->v - object.v, this->p - object.p) / pow((this->p - object.p).length(), 2)));
				object.setMove(object.v - (object.p - this->p) * (mass * Vector2D::Dot(object.v - this->v, object.p - this->p) / pow((object.p - this->p).length(), 2)));

				object.v = object.v * this->ballControl;
				if (this->v.length() > this->getVMax())
					this->setMove(Vector2D::Normal(this->v) * this->getVMax());
				
				if (object.v.length() > object.getVMax())
					object.setMove(Vector2D::Normal(object.v) * object.getVMax());
			}
		}
	}
}

void CirclePhysical::velocityAdd(Vector2D velocity)
{
	this->v = this->v + velocity;
}

float CirclePhysical::getVMax()
{
	return this->vMax;
}


int CirclePhysical::getBallControl()
{
	return this->ballControl;
}

float CirclePhysical::getFriction()
{
	return this->friction;
}

GameEngine* CirclePhysical::getGame()
{
	return this->pGame;
}

float CirclePhysical::getHitbox()
{
	return this->hitbox;
}

int CirclePhysical::getNumber()
{
	return this->number;
}

int CirclePhysical::getSize()
{
	return this->size;
}

float CirclePhysical::getWeight()
{
	return this->weight;
}

Vector2D CirclePhysical::getPosition()
{
	return this->p;
}


Vector2D CirclePhysical::getMove()
{
	return this->v;
}
