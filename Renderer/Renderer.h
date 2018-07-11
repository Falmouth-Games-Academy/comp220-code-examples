#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include <string>

struct RendererDesc
{
	std::string Type;
	int BackBufferWidth;
	int BackBufferHeight;
};

class IRenderer
{
public:
	virtual ~IRenderer() {};

	virtual bool Create(RendererDesc &desc,SDL_Window * window) = 0;

	virtual void Clear(int r, int g, int b) = 0;
	virtual void Begin() = 0;
	virtual void End() = 0;
};