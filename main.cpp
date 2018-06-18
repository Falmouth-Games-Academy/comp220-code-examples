#include <iostream>
#include "BaseGame\BaseGame.h"

int main(int argc, char ** argsv)
{
	BaseGame * MyGame = new BaseGame();

	if (!MyGame->Init(argc,argsv))
	{
		delete MyGame;
		MyGame = nullptr;
	}

	MyGame->Run();

	if (MyGame)
	{
		delete MyGame;
		MyGame = nullptr;
	}
	return 0;
}