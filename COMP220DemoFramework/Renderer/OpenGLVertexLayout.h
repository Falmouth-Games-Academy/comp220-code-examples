#pragma once

#include "VertexLayout.h"
#include "VertexBuffer.h"

#include <GL\glew.h>
#include <SDL_opengl.h>

class OpenGLVertexLayout :public IVertexLayout
{
public:
	OpenGLVertexLayout();
	~OpenGLVertexLayout();

	void CreateFromDescription(VertexDesc * desc, int numberOfElements);

	void BindVertexBuffer(IVertexBuffer * buffer);

	GLenum VertexDataTypeToGLType(VertexDataTypes type);

	void Bind();
	void UnBind();

	void Destroy();
private:
	GLuint VertexArrayObject;
	VertexDesc * VertexDescription;
	int NumberOfVertexDescriptions;
};