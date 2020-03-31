#pragma once

#include <gl\glew.h>
#include <SDL_opengl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

class Shader
{
public:
	Shader();
	~Shader();

	void Bind()
	{
		glUseProgram(ShaderProgramID);
	};

	bool Load(const std::string& vertShaderFilename, const std::string& fragShaderFilename);

	GLint GetUniform(const std::string& uniformName)
	{
		return Uniforms[uniformName];
	};

private:
	void RetrieveUniforms();
	GLuint ShaderProgramID;
	std::map<std::string, GLint> Uniforms;
};