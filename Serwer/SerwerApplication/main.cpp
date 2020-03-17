#include "stdafx.h"
using namespace  std;

int main()
{
	GameEngine game = GameEngine();

	for(int i=0;i<255;i++)
	{
		game.redraw();
		if(i%10==0)
			game.getBall()->setMove(Vector2D(3, 3));
		if (i == 50)
			int a = 0;
		if (i == 100)
			int a = 0;
		if (i == 125)
			int a = 0;
		if (i == 150)
			int a = 0;
		if (i == 200)
			int a = 0;
	}

	return 1;
}