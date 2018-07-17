#include "OpenGLRenderer.h"
#include "OpenGLVertexBuffer.h"

#include <SDL_opengl.h>
#include <SDL.h>
#include <GL\glew.h>

const OpenGLVersion OpenGLVersion::SupportedOpenGLVersions[]=
{
	//Open GL Versions in reverse order from best to worst
	{ 4,6 },
	{ 4,5 },	
	{ 4,4 },
	{ 4,3 },
	{ 4,2 },
	{ 4,1 },
	{ 4,0 },
	{ 3,3 },
	{ 3,2 },
	{ 3,1 },
	{ 3,0 }
};

OpenGLRender::OpenGLRender()
{
	Context = nullptr;
}

OpenGLRender::~OpenGLRender()
{

}

bool OpenGLRender::Create(RendererDesc &desc, SDL_Window * window)
{
	//Cache our window for later use
	Window = window;
	//Initialise context to nullptr
	SDL_GLContext context = nullptr;

	//Ask for a core Profile from SDL/OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Loop through each OpenGL version and try and create a context
	for (int i =0;i<sizeof(OpenGLVersion::SupportedOpenGLVersions) / sizeof(OpenGLVersion);i++)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OpenGLVersion::SupportedOpenGLVersions[i].MajorVersion);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OpenGLVersion::SupportedOpenGLVersions[i].MinorVersion);
		
		//Create a context, if it fails then this version of OpenGL is not supported by graphics card
		std::cout << "Attempting to create a " << OpenGLVersion::SupportedOpenGLVersions[i].MajorVersion << "." << OpenGLVersion::SupportedOpenGLVersions[i].MinorVersion << " Context" << std::endl;
		context= SDL_GL_CreateContext(window);
		if (context != nullptr)
		{
			std::cout << "Createed a " << OpenGLVersion::SupportedOpenGLVersions[i].MajorVersion << "." << OpenGLVersion::SupportedOpenGLVersions[i].MinorVersion << " Context" << std::endl;
			Context = context;
			break;
		}
		else
		{
			SDL_GL_DeleteContext(context);
		}

	}

	//If we didn't create an OpenGL context then something went wrong here!
	if (Context == nullptr)
	{
		std::cout << "Unable to create OpenGL Context" << std::endl;
		return false;
	}

	//Initialise GLEW, after we have created a context
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		std::cout << "Error initializing GLEW - " << glewGetErrorString(glewError) << std::endl;
	}

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//Display some info about the Graphics device
	std::cout<< "Renderer has been created" << std::endl; 
	std::cout<< "=========================" << std::endl;
	std::cout<< glGetString(GL_VENDOR)<<std::endl;
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	
	return true;
}

void OpenGLRender::Clear(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0);
}

void OpenGLRender::Begin()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRender::End()
{
	SDL_GL_SwapWindow(Window);
}

IVertexBuffer * OpenGLRender::CreateAndFillVertexBuffer(void ** data, int size)
{
	//Create an OpenGL Buffer and fill it with data
	OpenGLVertexBuffer * vertexBuffer = new OpenGLVertexBuffer();
	vertexBuffer->CreateAndFill(data, size);
	return vertexBuffer;
}

IVertexBuffer * OpenGLRender::CreateVertexBuffer(int size)
{
	//Create an OpenGL Buffer
	OpenGLVertexBuffer * vertexBuffer = new OpenGLVertexBuffer();
	vertexBuffer->Create(size);
	return vertexBuffer;
}
