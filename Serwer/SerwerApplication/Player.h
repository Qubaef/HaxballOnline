
/**
 *  The following class describes a single player during the game. 
 *  This class is a derived class and inherits from the "CirclePhysical" base class.
 */

#pragma once
#include "stdafx.h"

class Player:public CirclePhysical
{
private:
	short int team;		// to which team the player belongs

public:
	Player(GameEngine* game, int px, int py, int number);
	~Player();

	// getting team number 
	short int getTeam();

	// setting player's team
	void setTeam(short int team);	

	// calculating in which direction and how hard the player "kicked" the ball he's got ( using mouse pointer )
	void kick(double mouseX, double mouseY);

	// setup functions 
	void modeBallControl();
	void modeNormal();
};

