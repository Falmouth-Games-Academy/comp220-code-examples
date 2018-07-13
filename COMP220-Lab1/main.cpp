#include <iostream>

#include "Lab1Game.h"

int main(int argc, char ** argsv)
{
	BaseGame * MyGame = new Lab1Game();

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