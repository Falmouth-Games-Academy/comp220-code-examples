#include "OpenGLShader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

OpenGLShader::OpenGLShader()
{
	ShaderID = 0;
}

OpenGLShader::~OpenGLShader()
{
	Destroy();
}

bool OpenGLShader::Create(ShaderType type)
{
	ShaderID = glCreateShader(OpenGLShader::ShaderTypeToGLShaderType(type));
	CurrentShaderType = type;
	return true;
}

bool OpenGLShader::LoadFromMemory(const char * data)
{
	glShaderSource(ShaderID, 1, &data, NULL);
	return true;
}

bool OpenGLShader::LoadFromString(std::string & filename)
{
	std::ifstream fileStream(filename.c_str(), std::ifstream::in);

	std::stringstream contents;
	std::string line;


	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		contents << line<<"\n";
	}

	fileStream.close();

	LoadFromMemory(contents.str().c_str());
	return true;
}

bool OpenGLShader::Compile()
{
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glCompileShader(ShaderID);

	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0) {
		std::vector<char> errorLog(InfoLogLength);
		glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &errorLog[0]);
		
	}
	return true;
}

void OpenGLShader::Destroy()
{
	glDeleteShader(ShaderID);
}

GLenum OpenGLShader::ShaderTypeToGLShaderType(ShaderType type)
{
	GLenum glShaderType = GLenum();

	switch (type)
	{
		case COMPUTE:
			glShaderType = GL_COMPUTE_SHADER;
			break;
		case VERTEX:
			glShaderType = GL_VERTEX_SHADER;
			break;
		case TESSELATION_CONTROL:
			glShaderType = GL_TESS_CONTROL_SHADER;
			break;
		case TESSELATION_EVALUATION:
			glShaderType = GL_TESS_EVALUATION_SHADER;
			break;
		case FRAGMENT:
			glShaderType = GL_FRAGMENT_SHADER;
			break;
		case GEOMETRY:
			glShaderType = GL_GEOMETRY_SHADER;
			break;
	}

	return glShaderType;
}
