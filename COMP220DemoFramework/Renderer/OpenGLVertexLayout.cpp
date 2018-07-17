#include "OpenGLVertexLayout.h"

OpenGLVertexLayout::OpenGLVertexLayout()
{
	VertexArrayObject = 0;
}

OpenGLVertexLayout::~OpenGLVertexLayout()
{
	Destroy();
}

void OpenGLVertexLayout::CreateFromDescription(VertexDesc * desc, int numberOfElements)
{
	glGenVertexArrays(1, &VertexArrayObject);
	glBindVertexArray(VertexArrayObject);

	VertexDescription= desc;
	NumberOfVertexDescriptions= numberOfElements;
}

void OpenGLVertexLayout::BindVertexBuffer(IVertexBuffer * buffer)
{
	glBindVertexArray(VertexArrayObject);
	buffer->Bind();

	for (int i = 0; i < NumberOfVertexDescriptions; i++)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, VertexDescription[i].size, VertexDataTypeToGLType(VertexDescription[i].type), GL_FALSE, VertexDescription[i].stride, (void**)VertexDescription[i].offset);
	}
}

GLenum OpenGLVertexLayout::VertexDataTypeToGLType(VertexDataTypes type)
{
	GLenum glType = GLenum();

	switch (type)
	{
		case BYTE:
			glType = GL_BYTE;
			break;
		case UNSIGNED_BYTE:
			glType = GL_UNSIGNED_BYTE;
			break;
		case SHORT:
			glType = GL_SHORT;
			break;
		case UNSIGNED_SHORT:
			glType = GL_UNSIGNED_SHORT;
			break;
		case INT:
			glType = GL_INT;
			break;
		case UNSIGNED_INT:
			glType = GL_UNSIGNED_INT;
			break;
		case FLOAT:
			glType = GL_FLOAT;
			break;
		case DOUBLE:
			glType = GL_DOUBLE;
			break;
	}
	return type;
}

void OpenGLVertexLayout::Bind()
{
	glBindVertexArray(VertexArrayObject);
}

void OpenGLVertexLayout::UnBind()
{
	glBindVertexArray(0);
}

void OpenGLVertexLayout::Destroy()
{
	glDeleteVertexArrays(1, &VertexArrayObject);
}
