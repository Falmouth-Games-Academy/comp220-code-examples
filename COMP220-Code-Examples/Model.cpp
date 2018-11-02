#include "Model.h"

bool loadModelFromFile(const std::string& filename, GLuint VBO, GLuint EBO, unsigned int& numVerts, unsigned int& numIndices)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);
	if (!scene)
	{
		printf("Model Loading Error - %s\n", importer.GetErrorString());
		return false;
	}

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh *currentMesh = scene->mMeshes[i];

		for (int v = 0; v < currentMesh->mNumVertices; v++)
		{
			aiVector3D currentModelVertex = currentMesh->mVertices[v];
			aiColor4D currentModelColour = aiColor4D(1.0, 1.0, 1.0, 1.0);
			aiVector3D currentTextureCoordinates = aiVector3D(0.0f, 0.0f, 0.0f);
			aiVector3D currentModelNormals = aiVector3D(0.0f, 0.0f, 0.0f);

			if (currentMesh->HasVertexColors(0))
			{
				currentModelColour = currentMesh->mColors[0][v];
			}
			if (currentMesh->HasTextureCoords(0))
			{
				currentTextureCoordinates = currentMesh->mTextureCoords[0][v];
			}
			if (currentMesh->HasNormals())
			{
				currentModelNormals=currentMesh->mNormals[v];
			}
			Vertex currentVertex = { currentModelVertex.x,currentModelVertex.y,currentModelVertex.z,
				currentModelColour.r,currentModelColour.g,currentModelColour.b,currentModelColour.a,
				currentTextureCoordinates.x,currentTextureCoordinates.y,
				currentModelNormals.x,currentModelNormals.y,currentModelNormals.z};

			vertices.push_back(currentVertex);
		}

		for (int f = 0; f < currentMesh->mNumFaces; f++)
		{
			aiFace currentModelFace = currentMesh->mFaces[f];
			indices.push_back(currentModelFace.mIndices[0]);
			indices.push_back(currentModelFace.mIndices[1]);
			indices.push_back(currentModelFace.mIndices[2]);
		}
	}

	numVerts = vertices.size();
	numIndices = indices.size();

	// Give our vertices to OpenGL.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	return true;
}

bool loadMeshFromFile(const std::string & filename, MeshCollection * pMeshCollection)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);

	if (!scene)
	{
		printf("Model Loading Error - %s\n", importer.GetErrorString());
		return false;
	}

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh *currentMesh = scene->mMeshes[i];
		Mesh *pMesh = new Mesh();
		pMesh->init();

		for (int v = 0; v < currentMesh->mNumVertices; v++)
		{
			aiVector3D currentModelVertex = currentMesh->mVertices[v];
			aiColor4D currentModelColour = aiColor4D(1.0, 1.0, 1.0, 1.0);
			aiVector3D currentTextureCoordinates = aiVector3D(0.0f, 0.0f, 0.0f);
			aiVector3D currentModelNormals = aiVector3D(0.0f, 0.0f, 0.0f);

			if (currentMesh->HasVertexColors(0))
			{
				currentModelColour = currentMesh->mColors[0][v];
			}
			if (currentMesh->HasTextureCoords(0))
			{
				currentTextureCoordinates = currentMesh->mTextureCoords[0][v];
			}
			if (currentMesh->HasNormals())
			{
				currentModelNormals = currentMesh->mNormals[v];
			}
			Vertex currentVertex = { currentModelVertex.x,currentModelVertex.y,currentModelVertex.z,
				currentModelColour.r,currentModelColour.g,currentModelColour.b,currentModelColour.a,
				currentTextureCoordinates.x,currentTextureCoordinates.y,
				currentModelNormals.x,currentModelNormals.y,currentModelNormals.z };

			vertices.push_back(currentVertex);
		}

		for (int f = 0; f < currentMesh->mNumFaces; f++)
		{
			aiFace currentModelFace = currentMesh->mFaces[f];
			indices.push_back(currentModelFace.mIndices[0]);
			indices.push_back(currentModelFace.mIndices[1]);
			indices.push_back(currentModelFace.mIndices[2]);
		}

		pMesh->copyBufferData(vertices.data(), vertices.size(), indices.data(), indices.size());

		pMeshCollection->addMesh(pMesh);
		vertices.clear();
		indices.clear();
	}

	return true;
}