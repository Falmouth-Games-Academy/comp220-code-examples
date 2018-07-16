#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include <string>
#include "VertexBuffer.h"

//Structure to make it easier to describe a renderer
struct RendererDesc
{
	std::string Type;
	int BackBufferWidth;
	int BackBufferHeight;
};

//Pure virtual interface for a renderer. All renderers should inheirt from this
class IRenderer
{
public:
	virtual ~IRenderer() {};

	//Create Renderer
	virtual bool Create(RendererDesc &desc,SDL_Window * window) = 0;

	//Clear current display buffer
	virtual void Clear(float r, float g, float b) = 0;
	
	//begin rendering
	virtual void Begin() = 0;
	//end rendering
	virtual void End() = 0;

	//Create and fill a vertex buffer, please note, you will have to destory the buffer
	virtual IVertexBuffer * CreateAndFillVertexBuffer(void ** data, int size)=0;
	//Create a vertex buffer, please note, you will have to destory the  buffer
	virtual IVertexBuffer * CreateVertexBuffer(int size) = 0;

};