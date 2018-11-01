#pragma once

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <vector>

#include "vertex.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void init();
	void copyBufferData(Vertex *pVerts, unsigned int numberOfVerts, unsigned int *pIndices, unsigned int numberOfIndices);
	void render();
	void destroy();
private:
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_VAO;
	unsigned int m_NumberOfVertices;
	unsigned int m_NumberOfIndices;
};

class MeshCollection
{
public:
	MeshCollection();
	~MeshCollection();

	void addMesh(Mesh *pMesh);

	void render();
	void destroy();
private:
	std::vector<Mesh*> m_Meshes;
}; 
