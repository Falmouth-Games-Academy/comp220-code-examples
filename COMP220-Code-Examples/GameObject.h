#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"
#include "Mesh.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Destroy();

	bool LoadMesh(const std::string& filename);

	void SetPosition(float x, float y, float z)
	{
		Position = glm::vec3(x, y, z);
	};

	glm::vec3& GetPosition()
	{
		return Position;
	};

	void SetScale(float x, float y, float z)
	{
		Scale = glm::vec3(x, y, z);
	};

	glm::vec3& GetScale()
	{
		return Scale;
	};

	void SetRotation(float x, float y, float z)
	{
		Rotation = glm::vec3(x, y, z);
	};

	glm::vec3& GetRotation()
	{
		return Rotation;
	};

	glm::mat4 GetMatModel()
	{
		return MatModel;
	};
protected:
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	glm::mat4 MatTranslation;
	glm::mat4 MatRotation;
	glm::mat4 MatScale;

	glm::mat4 MatModel;

	MeshCollection* Meshes;
};