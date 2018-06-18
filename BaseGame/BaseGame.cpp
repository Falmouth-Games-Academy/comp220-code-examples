#include "BaseGame.h"


BaseGame::BaseGame()
{
	Window = nullptr;
	Renderer = nullptr;
	Running = false;
}

BaseGame::~BaseGame()
{
	Shutdown();
}

bool BaseGame::Init(int argc, char ** argsv)
{
	//Initialise SDL Library,, if we fail display an error message to the console
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		std::cout << "Can't initialise SDL " << SDL_GetError() << std::endl;
		return false;
	}

	//use image flags to request support for the following image formats
	int imgFlag = IMG_INIT_PNG | IMG_INIT_JPG;
	//Initialise the IMG library and check the results
	int imgFlagResults = IMG_Init(imgFlag);
	if (imgFlagResults != imgFlag)
	{
		//We failed then we can only use BMPs
		std::cout << "Can't load in PNG and JPG support, we can use BMPs " << IMG_GetError() << std::endl;
	}
	ParseCommandLineArgs(argc, argsv);
	if (!CreateWindow(1240, 720, "BaseGame", false))
		return false;

	if (!CreateRenderer())
		return false;

	Running = true;

	return true;
}

bool BaseGame::CreateWindow(int width, int height, const std::string & title, bool fullScreen)
{
	//Create a winodw, with a title and dimensions specfied. The last value is the flags, which we will revisit
	Uint32 flags = SDL_WINDOW_SHOWN;
	if (fullScreen)
	{
		flags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN;
	}
	Window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
	//Check to see if the window has been created
	if (Window == nullptr)
	{
		std::cout << "Can't create Window" << SDL_GetError() << std::endl;
		Shutdown();
		return false;
	}
	return true;
}

bool BaseGame::CreateRenderer()
{
	//Create Hardware Renderer
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

	if (Renderer == nullptr)
	{
		std::cout << "Can't create renderer " << SDL_GetError() << std::endl;
		Shutdown();
		return false;
	}
	return true;
}

void BaseGame::ParseCommandLineArgs(int argc, char ** argsv)
{
}

void BaseGame::Shutdown()
{
	//Destroy Renderer
	SDL_DestroyRenderer(Renderer);
	//Destroy the Window
	SDL_DestroyWindow(Window);
	//Quit Image
	IMG_Quit();
	//Quit SDL
	SDL_Quit();
}

void BaseGame::Update(float updateTime)
{
}

void BaseGame::Render()
{
	SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
	SDL_RenderClear(Renderer);


	SDL_RenderPresent(Renderer);
}

void BaseGame::Run()
{
	while (Running)
	{
		//SDL Event pump, keep Polling while we have events to handle
		while (SDL_PollEvent(&CurrentEvent))
		{
			//Check the type of event
			switch (CurrentEvent.type)
			{
				//If this is a quit message then stop the BaseGame loop
			case SDL_QUIT:
			{
				Running = false;
				break;
			}
			}
		}

		ProcessInput();

		Update(0.0f);

		Render();
	}
}

void BaseGame::ProcessInput()
{
}


