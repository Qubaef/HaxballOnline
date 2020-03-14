#include "stdafx.h"
using namespace  std;

int main()
{
	GameEngine game = GameEngine();

	while(true)
	{
		game.redraw();
		game.getBall()->setMove(Vector2D(-1, 0));
	}

	return 1;
}