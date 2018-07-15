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
	
	ParseConfigFile();
	ParseCommandLineArgs(argc, argsv);

	//Grab standard options from our Config class
	WindowDescription.WindowTitle = ConfigOptions.GetOption("window", "title", "Game");
	WindowDescription.FullScreenWindow = ConfigOptions.GetOptionAsBool("window","fullscreen",false);
	WindowDescription.WindowHeight = ConfigOptions.GetOptionAsInt("window","height",1080);
	WindowDescription.WindowWidth = ConfigOptions.GetOptionAsInt("window","width",1960);

	RendererDescription.Type = ConfigOptions.GetOption("renderer","type","OpenGL");
	RendererDescription.BackBufferWidth = WindowDescription.WindowWidth;
	RendererDescription.BackBufferHeight = WindowDescription.WindowHeight;

	//Create the Window
	if (!CreateWindow(WindowDescription))
		return false;

	//Create the Renderer
	if (!CreateRenderer())
		return false;

	MainTimer.Start();

	Running = true;

	return true;
}

bool BaseGame::CreateWindow(WindowDesc &desc)
{
	//Create a winodw, with a title and dimensions specfied. The last value is the flags, which we will revisit
	Uint32 flags = SDL_WINDOW_SHOWN;
	if (desc.FullScreenWindow)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	
	if (RendererDescription.Type == "OpenGL")
	{
		flags |= SDL_WINDOW_OPENGL;
	}

	//Create an SDL window with the passed in options
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
	//Are we going to create an OpenGL Renderer
	if (RendererDescription.Type == "OpenGL")
	{
		Renderer = new OpenGLRender();
	}

	//Check for errors in initialisation and create Renderer
	if (Renderer != nullptr)
	{
		Renderer->Create(RendererDescription, Window);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplSdlGL3_Init(Window, 0);

	return true;
}

void BaseGame::ParseCommandLineArgs(int argc, char ** argsv)
{
	ConfigOptions.ParseCommandLine(argc, argsv);
}

void BaseGame::ParseConfigFile()
{
	ConfigOptions.ParseFile("config.ini");
}

void BaseGame::Shutdown()
{
	ImGui_ImplSdlGL3_Shutdown();
	//Delete everything in reverse order of initialisation
	if (Renderer)
	{
		delete Renderer;
		Renderer = nullptr;
	}
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
	bool show = true;

	ImGui_ImplSdlGL3_NewFrame(Window);
	
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(WindowDescription.WindowWidth, WindowDescription.WindowHeight));
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("Debug", &show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
	ImGui::Text("%.2f",(1.0f/MainTimer.GetDeltaTime()));
	ImGui::End();

	ImGui::Render();

	//Clear and begin rendering
	Renderer->Clear(1.0f, 0.0f, 1.0f);
	Renderer->Begin();
	ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
	Renderer->End();
}

void BaseGame::Run()
{
	while (Running)
	{
		//SDL Event pump, keep Polling while we have events to handle
		while (SDL_PollEvent(&CurrentEvent))
		{
			ImGui_ImplSdlGL3_ProcessEvent(&CurrentEvent);
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

		MainTimer.Update();
		///Process user Input
		ProcessInput();

		//Update Game
		Update(MainTimer.GetDeltaTime());

		//Render Game
		Render();
	}
}

void BaseGame::ProcessInput()
{
}

void BaseGame::InitScene()
{

}


