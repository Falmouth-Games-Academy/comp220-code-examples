#include "OpenGLShaderProgram.h"
#include "OpenGLShader.h"
#include <vector>

OpenGLShaderProgram::OpenGLShaderProgram()
{
}

OpenGLShaderProgram::~OpenGLShaderProgram()
{
	Destroy();
}

void OpenGLShaderProgram::AttachShader(IShader * shader)
{
	OpenGLShader * GLShader = dynamic_cast<OpenGLShader*>(shader);

	glAttachShader(ProgramID, GLShader->GetShaderID());

	AttachedShaders.push_back(shader);
}

bool OpenGLShaderProgram::Link()
{
	glLinkProgram(ProgramID);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	return true;
}

void OpenGLShaderProgram::Create()
{
	ProgramID = glCreateProgram();
}

void OpenGLShaderProgram::Destroy()
{
	glDeleteProgram(ProgramID);
}

void OpenGLShaderProgram::DetachShaders()
{
	for (IShader * shader : AttachedShaders) {
		OpenGLShader * GLShader = dynamic_cast<OpenGLShader*>(shader);

		glAttachShader(ProgramID, GLShader->GetShaderID());
	}
}
