#pragma once

enum VertexDataTypes
{
	BYTE=0,
	UNSIGNED_BYTE,
	SHORT,
	UNSIGNED_SHORT,
	INT,
	UNSIGNED_INT,
	FLOAT,
	DOUBLE,
};

struct VertexDesc
{
	int index;
	int size;
	VertexDataTypes type;
	int stride;
	int offset;
};

class IVertexBuffer;

class IVertexLayout
{
public:
	virtual ~IVertexLayout() {};

	virtual void CreateFromDescription(VertexDesc * desc, int numberOfElements)=0;

	virtual void BindVertexBuffer(IVertexBuffer * buffer)=0;

	virtual void Bind() = 0;
	virtual void UnBind() = 0;

	virtual void Destroy() = 0;
};