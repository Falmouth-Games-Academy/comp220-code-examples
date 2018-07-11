#pragma once

#include "Renderer.h"

struct OpenGLVersion
{
	int MajorVersion;
	int MinorVersion;

	const static OpenGLVersion SupportedOpenGLVersions[];
};

class OpenGLRender :public IRenderer
{
public:
	OpenGLRender();
	~OpenGLRender();

	bool Create(RendererDesc &desc, SDL_Window * window);

	void Clear(int r, int g, int b);
	void Begin();
	void End();
private:
	SDL_GLContext Context;
	SDL_Window * Window;
};