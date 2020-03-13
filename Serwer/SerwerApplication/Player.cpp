#include "stdafx.h"
#include "Ball.h"

Player::Player(GameEngine* pGame, int px, int py, int number)
	:CirclePhysical(pGame,px,py,number,1,15)
{

}

void Player::kick(Vector2D pos)
{
	for (Ball b : this->pGame->getObjects)
	{


	}
}

void Player::modeBalllControl()
{


}

void Player::modeNormal()
{

}

