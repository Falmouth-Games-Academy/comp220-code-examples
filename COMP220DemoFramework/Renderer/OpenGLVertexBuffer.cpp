#include "OpenGLVertexBuffer.h"


OpenGLVertexBuffer::OpenGLVertexBuffer()
{
	//Initial values
	VertexBufferID = 0;
	BufferSize = 0;
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	//Call destory just to be on the safe size
	Destroy();
}

bool OpenGLVertexBuffer::CreateAndFill(void ** data, int size)
{
	//Call our create function
	Create(size);
	//Copy the data into the buffer
	FillWithData(data, size);

	return true;
}

bool OpenGLVertexBuffer::Create(int size)
{
	//Generate 1 buffer
	glGenBuffers(1, &VertexBufferID);
	//Bind it as a Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	//Fill the buffer with zero data
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);

	return true;
}

void OpenGLVertexBuffer::Bind()
{
	//Bind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
}

void OpenGLVertexBuffer::UnBind()
{
	//unbind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::Destroy()
{
	//Delete the buffer
	glDeleteBuffers(1, &VertexBufferID);
}

void OpenGLVertexBuffer::FillWithData(void ** data, int size)
{
	//Copy the data into the buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
