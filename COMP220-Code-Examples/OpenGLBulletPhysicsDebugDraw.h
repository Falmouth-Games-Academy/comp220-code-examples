#pragma once

#include <GL\glew.h>
#include <SDL_opengl.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <btBulletDynamicsCommon.h>

#include "Shaders.h"

class OpenGLBulletPhysicsDebugDraw :public btIDebugDraw
{
public:
	OpenGLBulletPhysicsDebugDraw();
	~OpenGLBulletPhysicsDebugDraw();

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	void CreateDebugShader();

	void SetViewProjectionMatrix(glm::mat4& view, glm::mat4 & projection)
	{
		ViewMatrix = view;
		ProjectMatrix = projection;
	};

	virtual void drawContactPoint(const btVector3 &,const btVector3 &,btScalar,int,const btVector3 &){}
	virtual void reportErrorWarning(const char *){}
	virtual void draw3dText(const btVector3 &,const char *){}
	virtual void setDebugMode(int p){
		mode = p;
	}
	
	int getDebugMode() const
	{
		return 3;
	};

private:
	int mode;
	GLuint VBO;
	GLuint VAO;

	Shader * DebugShader;

	glm::mat4 ModelMatrix, ViewMatrix, ProjectMatrix;
};