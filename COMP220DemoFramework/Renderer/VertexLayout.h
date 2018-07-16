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

class IVertexLayout
{
public:
	virtual ~IVertexLayout() {};


};