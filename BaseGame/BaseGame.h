#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>


class BaseGame
{
public:
	BaseGame();
	virtual ~BaseGame();

	virtual bool Init(int argc, char ** argsv);
	virtual void Shutdown();

	void Run();

	void ProcessInput();

	virtual void Update(float updateTime);
	virtual void Render();

	bool IsRunning()
	{
		return Running;
	};
private:
	bool CreateWindow(int width, int height, const std::string& title, bool fullScreen);
	bool CreateRenderer();
	void ParseCommandLineArgs(int argc, char ** argsv);
protected:
	SDL_Window * Window;
	SDL_Renderer * Renderer;
	SDL_Event CurrentEvent;
	bool Running;
};