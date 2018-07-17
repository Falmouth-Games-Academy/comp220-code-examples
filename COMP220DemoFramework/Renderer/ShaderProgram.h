#pragma once

#include "Shader.h"

class IShaderProgram
{
public:
	virtual ~IShaderProgram(){};

	virtual void Create() = 0;
	virtual void AttachShader(IShader * shader) = 0;
	virtual void DetachShaders() = 0;
	virtual bool Link() = 0;
	virtual void Destroy() = 0;
};