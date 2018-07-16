#pragma once

//Interface class to represent a buffer which contains vertex data
class IVertexBuffer
{
public:
	//We must always define an empty deconstructor for Interfaces
	virtual ~IVertexBuffer(){};

	//Create and Fill the vertex buffer with data
	virtual bool CreateAndFill(void ** data, int size) = 0;
	//Create Vertex Buffer
	virtual bool Create(int size) = 0;
	//Fill Vertex buffer with data
	virtual void FillWithData(void **data, int size) = 0;
	//Bind to pipeline
	virtual void Bind() = 0;
	//Unbind from the pipeline
	virtual void UnBind() = 0;
	//Destroy
	virtual void Destroy() = 0;
};