#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <btBulletDynamicsCommon.h>

#include "Mesh.h"
#include "Shaders.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update(float deltaTime);

	void SetScale(float x, float y, float z)
	{
		Scale = glm::vec3(x, y, z);
	};

	void SetPosition(float x, float y, float z)
	{
		Position = glm::vec3(x, y, z);
		if (RigidBody)
		{
			btTransform transform=RigidBody->getCenterOfMassTransform();
			transform.setOrigin(btVector3(x, y, z));
			RigidBody->setCenterOfMassTransform(transform);
			RigidBody->clearForces();
		}
	};

	glm::vec3& GetPosition()
	{
		return Position;
	};

	glm::mat4& GetModelTransformation()
	{
		return ModelMatrix;
	};

	void SetMesh(MeshCollection * meshes)
	{
		Meshes = meshes;
	};

	void SetShader(Shader * shader)
	{
		ShaderProgram = shader;
	};

	Shader * GetShader()
	{
		return ShaderProgram;
	};

	void SetDiffuseTexture(GLuint texture)
	{
		DiffuseTexture = texture;
	};

	GLuint GetDiffuseTexture()
	{
		return DiffuseTexture;
	};

	void SetRigidBody(btRigidBody* rigidBody)
	{
		RigidBody = rigidBody;
	};

	btRigidBody*  GetRigidBody()
	{
		return RigidBody;
	};

	void SetName(const std::string& name)
	{
		Name = name;
	};

	const std::string& GetName()
	{
		return Name;
	};

	void Render();

private:
	//Set up positions for position, rotation and scale
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	//calculate the translation, rotation and scale matrices using the above vectores
	glm::mat4 TranslationMatrix;
	glm::mat4 RotationMatrix;
	glm::mat4 ScaleMatrix;
	//combine the above matrices into the model matrix (order is important!!!! - TRS)
	glm::mat4 ModelMatrix;

	//Mesh
	MeshCollection * Meshes;

	//Shader
	Shader * ShaderProgram;

	//Texture
	GLuint DiffuseTexture;

	//RigidBody
	btRigidBody * RigidBody;

	std::string Name;
};