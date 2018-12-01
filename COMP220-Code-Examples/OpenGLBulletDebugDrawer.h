#pragma once

#include <GL\glew.h>
#include <SDL_opengl.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <btBulletDynamicsCommon.h>

#include "Vertex.h"
#include "Shaders.h"

class OpenGLBulletDebugDrawer :public btIDebugDraw
{
public:
	OpenGLBulletDebugDrawer();
	~OpenGLBulletDebugDrawer();

	void CreateShader();
	
	void SetViewAndProjectMatrix(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix);

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	void drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int, const btVector3 &) {}
	void reportErrorWarning(const char *) {}
	void draw3dText(const btVector3 &, const char *) {}

	virtual void setDebugMode(int p) {
		DebugMode = p;
	}

	int getDebugMode(void) const { return 3; }

private:
	GLuint VBO;
	GLuint VAO;
	Shader * DebugShader;
	int DebugMode;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ModelMatrix;
};