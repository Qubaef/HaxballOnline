#include "stdafx.h"
#include "Ball.h"
#include "Vector2D.h"

Player::Player(GameEngine* pGame, int px, int py, int number)
	:CirclePhysical(pGame, px, py, number, 1, 15)
{
	this->team = -1;
}


Player::~Player()
{
}


void Player::kick(Vector2D pos)
{
	//for (Ball b : this->pGame->getBall)
	Ball* b = this->pGame->getBall();
	Vector2D v = pos - b->getMove();


	float dist = pow(this->getPosition().getX() - b->getPosition().getX(), 2);
	dist += pow(b->getPosition().getY() - this->getPosition().getY(), 2);

	if (dist <= pow(b->getHitbox() + this->getHitbox(), 2))
	{
		//kick ball to given position
		v = v.Normal(v) * v.length() / 12;
		b->setMove(v);
	}
}


void Player::modeBallControl()
{
	//turn down ball_control to reduce the bounce
	this->ballControl = 0.1;
	this->vMax = 2 / pow(this->getWeight(), 2 / 3);
}


void Player::modeNormal()
{
	// bring back normal mode
	this->ballControl = 1.0;
	this->vMax = 6 / pow(this->getWeight(), 2 / 3);
}


short int Player::getTeam()
{
	return team;
}


void Player::setTeam(short int team)
{
	this->team = team;
}