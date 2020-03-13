#pragma once
#include "stdafx.h"
class Goal
{
private:
	GameEngine* pGame;
	int x;
	int yUp;
	int yDown;
	int width;
	int direction;
	Post *pPostUp;
	Post *pPostDown;
public:
	Goal(GameEngine* pGame, int x, int yUp, int yDown, int width, int direction, Post* pPostUp, Post* pPostDown);
	
};

