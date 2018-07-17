#pragma once

#include <string>

enum ShaderType
{
	COMPUTE=0,
	VERTEX,
	TESSELATION_CONTROL,
	TESSELATION_EVALUATION,
	GEOMETRY,
	FRAGMENT
};

class IShader
{
public:
	virtual ~IShader() {};

	virtual bool Create(ShaderType type) = 0;
	virtual bool LoadFromMemory(const char * data) = 0;
	virtual bool LoadFromString(std::string& filename) = 0;
	virtual bool Compile() = 0;
	virtual void Destroy() = 0;
	
	ShaderType GetType()
	{
		return CurrentShaderType;
	};

protected:
	ShaderType CurrentShaderType;
};
