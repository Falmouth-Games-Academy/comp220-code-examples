#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <sstream>

#include "ProgramOptions.h"

//Struct to make it easier to pass around values for our Window
struct WindowDesc
{
	int WindowWidth;
	int WindowHeight;
	bool FullScreenWindow;
	std::string WindowTitle;

	WindowDesc()
	{
		WindowWidth = 1280;
		WindowHeight = 720;
		FullScreenWindow = false;
		WindowTitle = "Lab2";
	}
};

//Base Game Class
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
	bool CreateWindow(WindowDesc &desc);
	bool CreateRenderer();
	void ParseCommandLineArgs(int argc, char ** argsv);
	void ParseConfigFile();
protected:
	SDL_Window * Window;
	SDL_Renderer * Renderer;
	SDL_Event CurrentEvent;
	bool Running;
	ProgramOptions Options;
	WindowDesc WindowDescription;
};