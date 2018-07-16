#pragma once

#include "VertexBuffer.h"
#include <GL\glew.h>
#include <SDL_opengl.h>

//Concrete class for an OpenGL Vertex Buffer
class OpenGLVertexBuffer :public IVertexBuffer
{
public:
	//Consstructor
	OpenGLVertexBuffer();
	//Deconstructor
	~OpenGLVertexBuffer();

	//Create and Fill VBO
	bool CreateAndFill(void ** data, int size) override;
	//Create buffer
	bool Create(int size) override;

	//Bind the buffer to the pipeline
	void Bind() override;
	//Unbind the buffer from the pipeline
	void UnBind() override;
	//Destroy the buffer
	void Destroy() override;

	void FillWithData(void **data, int size) override;
private:
	//Buffer ID
	GLuint VertexBufferID;
	//Size of the buffer
	int BufferSize;
};