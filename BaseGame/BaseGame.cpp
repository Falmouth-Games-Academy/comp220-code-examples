#include "BaseGame.h"


BaseGame::BaseGame()
{
	Window = nullptr;
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
	
	ParseConfigFile();
	ParseCommandLineArgs(argc, argsv);

	//Grab standard options
	WindowDescription.WindowTitle = Options.GetOption("title");
	WindowDescription.FullScreenWindow = Options.GetOptionAsBool("fullscreen");
	WindowDescription.WindowHeight = Options.GetOptionAsInt("height");
	WindowDescription.WindowWidth = Options.GetOptionAsInt("width");

	if (!CreateWindow(WindowDescription))
		return false;

	if (!CreateRenderer())
		return false;

	Running = true;

	return true;
}

bool BaseGame::CreateWindow(WindowDesc &desc)
{
	//Create a winodw, with a title and dimensions specfied. The last value is the flags, which we will revisit
	Uint32 flags = SDL_WINDOW_SHOWN;
	if (desc.FullScreenWindow)
	{
		flags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN;
	}
	Window = SDL_CreateWindow(desc.WindowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, desc.WindowWidth, desc.WindowHeight, flags);
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
	return true;
}

void BaseGame::ParseCommandLineArgs(int argc, char ** argsv)
{
	// start at one as the exe is the first arg!
	for (int i = 1; i < argc; i++)
	{
		std::string optionValuePair = argsv[i];
		int delminatorPos = optionValuePair.find("=");
		std::string optionName = optionValuePair.substr(0, delminatorPos - 1);
		std::string optionValue = optionValuePair.substr(delminatorPos);
		Options.AddOption(optionName, optionValue);
	}
}

void BaseGame::ParseConfigFile()
{
	Options.ParseConfigFile("config.ini");
}

void BaseGame::Shutdown()
{
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


