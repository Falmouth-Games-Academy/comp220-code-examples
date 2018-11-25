#include "GameObject.h"

GameObject::GameObject()
{
	Position = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);
	Rotation = glm::vec3(0.0f);

	TranslationMatrix = glm::mat4(0.0f);
	RotationMatrix = glm::mat4(0.0f);
	ScaleMatrix = glm::mat4(0.0f);
	ModelMatrix = glm::mat4(0.0f);

	Meshes = nullptr;
	ShaderProgram = nullptr;
	DiffuseTexture = 0;
	RigidBody = nullptr;
}

GameObject::~GameObject()
{
	glDeleteTextures(1, &DiffuseTexture);

	if (RigidBody)
	{
		delete RigidBody->getCollisionShape();
		delete RigidBody->getMotionState();
		delete RigidBody;
	}
	if (Meshes) 
	{
		delete Meshes;
	}
	if (ShaderProgram)
	{
		delete ShaderProgram;
	}
}

void GameObject::Update(float deltaTime)
{
	if (RigidBody)
	{
		btTransform currentTransform;
		btMotionState* currentMotionState = RigidBody->getMotionState();
		currentMotionState->getWorldTransform(currentTransform);
		
		Position = glm::vec3(currentTransform.getOrigin().getX(),
			currentTransform.getOrigin().getY(),
			currentTransform.getOrigin().getZ());
	}
	TranslationMatrix = glm::translate(Position);
	RotationMatrix = glm::rotate(Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f))
		*glm::rotate(Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f))
		*glm::rotate(Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	ScaleMatrix = glm::scale(Scale);

	//combine the above matrices into the model matrix (order is important!!!! - TRS)
	ModelMatrix = TranslationMatrix * RotationMatrix*ScaleMatrix;
}

void GameObject::Render()
{
	if (Meshes)
	{
		Meshes->render();
	}
}
