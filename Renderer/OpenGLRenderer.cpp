#include "OpenGLRenderer.h"


const OpenGLVersion OpenGLVersion::SupportedOpenGLVersions[]=
{
	{3,0},
	{3,1},
	{3,2},
	{3,3},
	{4,0},
	{4,1},
	{4,2},
	{4,3},
	{4,4},
	{4,5},
	{4,6}
};

OpenGLRender::OpenGLRender()
{

}

OpenGLRender::~OpenGLRender()
{

}

bool OpenGLRender::Create(RendererDesc &desc, SDL_Window * window)
{
	Window = window;
	//initialise a dummy context first

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	for (int i = (sizeof(OpenGLVersion::SupportedOpenGLVersions) / sizeof(OpenGLVersion)) - 1; i < 0; i--)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OpenGLVersion::SupportedOpenGLVersions[i].MajorVersion);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OpenGLVersion::SupportedOpenGLVersions[i].MinorVersion);

		SDL_GLContext context= SDL_GL_CreateContext(window);
		if (context != nullptr)
		{
			Context = context;
			break;
		}
		else
		{
			SDL_GL_DeleteContext(context);
		}

	}

	glewExperimental = GL_TRUE;
	glewInit();

	//Create actual context
	return true;
}

void OpenGLRender::Clear(int r, int g, int b)
{

}

void OpenGLRender::Begin()
{

}

void OpenGLRender::End()
{

}