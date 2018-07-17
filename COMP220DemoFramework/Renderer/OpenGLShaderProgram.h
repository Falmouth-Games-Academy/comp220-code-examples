#pragma once

#include "ShaderProgram.h"
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <vector>

class OpenGLShaderProgram :public IShaderProgram
{
public:
	OpenGLShaderProgram();
	~OpenGLShaderProgram();

	// Inherited via IShaderProgram
	void AttachShader(IShader * shader) override;
	bool Link() override;
	void Create() override;
	void Destroy() override;
	void DetachShaders() override;
private:
	GLuint ProgramID;
	std::vector<IShader*> AttachedShaders;
};