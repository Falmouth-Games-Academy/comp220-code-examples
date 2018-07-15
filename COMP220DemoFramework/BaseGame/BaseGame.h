#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <sstream>

#include "Config.h"
#include "Timer.h"

#include "..\Renderer\Renderer.h"
#include "..\Renderer\OpenGLRenderer.h"

#include "IMGUI\imgui.h"
#include "IMGUI\imgui_impl_sdl_gl3.h"

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
	};
};

//Base Game Class
class BaseGame
{
public:
	BaseGame();
	virtual ~BaseGame();

	//Init the game, we are passing in command line arguments to change the options of the game
	virtual bool Init(int argc, char ** argsv);
	//Called when we shutdown
	virtual void Shutdown();

	//Run the game
	void Run();

	//Process User Input
	void ProcessInput();

	//Update the current game state
	virtual void Update(float updateTime);
	//Render the current game state
	virtual void Render();

	virtual void InitScene();

	//Check to see if the game is running
	bool IsRunning()
	{
		return Running;
	};
private:
	//Create a Window
	bool CreateWindow(WindowDesc &desc);
	//Create a Renderer
	bool CreateRenderer();
	//Parse Command line arguments
	void ParseCommandLineArgs(int argc, char ** argsv);
	//Parse Config file
	void ParseConfigFile();
protected:
	SDL_Window * Window;
	SDL_Event CurrentEvent;
	bool Running;
	WindowDesc WindowDescription;
	RendererDesc RendererDescription;
	IRenderer * Renderer;
	Config ConfigOptions;
	Timer MainTimer;

};