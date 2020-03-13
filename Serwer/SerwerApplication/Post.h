#pragma once
#include "stdafx.h"
class Post:public CirclePhysical
{
	Post(GameEngine* game, int px, int py);
	void update() override;
	void SetP(Vector2D p) override;
};

