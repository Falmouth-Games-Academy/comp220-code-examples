#include "GameObject.h"

GameObject::GameObject()
{
	Position = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);
	Rotation = glm::vec3(0.0f);

	MatTranslation = glm::mat4(1.0f);
	MatRotation = glm::mat4(1.0f);
	MatScale = glm::mat4(1.0f);
	MatModel = glm::mat4(1.0f);

	Meshes = nullptr;
}

GameObject::~GameObject()
{
}

void GameObject::Init()
{
}

void GameObject::Update()
{
	MatTranslation = glm::translate(Position);

	MatRotation = glm::rotate(Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::rotate(Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	MatScale = glm::scale(Scale);

	MatModel = MatTranslation * MatRotation * MatScale;
}

void GameObject::Render()
{
	Meshes->render();
}

void GameObject::Destroy()
{
	if (Meshes)
	{
		Meshes->destroy();
		delete Meshes;
	}
}

bool GameObject::LoadMesh(const std::string& filename)
{
	Meshes = new MeshCollection();
	loadMeshCollectionFromFile(filename, Meshes);
	
	return true;
}
