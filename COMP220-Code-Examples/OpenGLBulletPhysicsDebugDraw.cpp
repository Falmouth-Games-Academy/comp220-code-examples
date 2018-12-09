#include "OpenGLBulletPhysicsDebugDraw.h"

OpenGLBulletPhysicsDebugDraw::OpenGLBulletPhysicsDebugDraw()
{
	DebugShader = nullptr;

	ModelMatrix = glm::mat4(1.0f);
	ViewMatrix = glm::mat4(1.0f);
	ProjectMatrix = glm::mat4(1.0f);
}

OpenGLBulletPhysicsDebugDraw::~OpenGLBulletPhysicsDebugDraw()
{
	if (DebugShader)
	{
		delete DebugShader;
	}

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void OpenGLBulletPhysicsDebugDraw::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
{
	GLfloat points[14];

	points[0] = from.x();
	points[1] = from.y();
	points[2] = from.z();
	points[3] = color.x();
	points[4] = color.y();
	points[5] = color.z();
	points[6] = 1.0f;

	points[7] = to.x();
	points[8] = to.y();
	points[9] = to.z();
	points[10] = color.x();
	points[11] = color.y();
	points[12] = color.z();
	points[13] = 1.0f;

	//Delete buffers
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);


	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void**)(3*sizeof(GLfloat)));


	//Use a shader

	DebugShader->Use();

	glUniformMatrix4fv(DebugShader->GetUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniformMatrix4fv(DebugShader->GetUniform("viewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(DebugShader->GetUniform("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectMatrix));

	//Send model, view and project
	glDrawArrays(GL_LINES, 0, 2);


}

void OpenGLBulletPhysicsDebugDraw::CreateDebugShader()
{
	DebugShader = new Shader();
	DebugShader->Load("vert.glsl", "frag.glsl");
}
