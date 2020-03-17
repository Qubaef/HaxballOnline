#include "stdafx.h"
using namespace  std;

int main()
{
	GameEngine game = GameEngine();
	Player player = Player(&game, 550, 298, 1);
	game.newPlayer(&player, 1);
	for(int i=0;i<255;i++)
	{
		game.redraw();
		if(i%10==0)
			game.getBall()->setMove(Vector2D(3, 2));
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