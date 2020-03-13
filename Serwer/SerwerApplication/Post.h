#pragma once
#include "stdafx.h"
class Post:public CirclePhysical
{
public:
	Post(GameEngine* game, int px, int py);
	void update() override;
	void setPosition(Vector2D p) override;
	void setMove(Vector2D v)override;
};

