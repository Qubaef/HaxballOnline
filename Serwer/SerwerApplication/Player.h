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
	// calculates in which direction and how hard the player "kicked" the ball he got ( using mouse pointer )
	void kick(double mouseX, double mouseY);
	// setup functions 
	void modeBallControl();
	void modeNormal();
};

