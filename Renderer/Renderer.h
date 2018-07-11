#pragma once

struct RendererDesc
{
	int BackBufferWidth;
	int BackBufferHeight;
};

class IRenderer
{
public:
	virtual ~IRenderer() {};


};