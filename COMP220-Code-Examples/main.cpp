#include <iostream>
#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shaders.h"
#include "Vertex.h"
#include "Texture.h"
#include "Model.h"
#include "Mesh.h"
#include "Light.h"
#include "Timer.h"
#include "GameObject.h"

int main(int argc, char ** argsv)
{
	//Initialises the SDL Library, passing in SDL_INIT_VIDEO to only initialise the video subsystems
	//https://wiki.libsdl.org/SDL_Init
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//Display an error message box
		//https://wiki.libsdl.org/SDL_ShowSimpleMessageBox
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Init failed", SDL_GetError(), NULL);
		return 1;
	}

	//Create a window, note we have to free the pointer returned using the DestroyWindow Function
	//https://wiki.libsdl.org/SDL_CreateWindow
	SDL_Window* window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 640, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	//Checks to see if the window has been created, the pointer will have a value of some kind
	if (window == nullptr)
	{
		//Show error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateWindow failed", SDL_GetError(), NULL);
		//Close the SDL Library
		//https://wiki.libsdl.org/SDL_Quit
		SDL_Quit();
		return 1;
	}

	//Request 3.2 Core OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GLContext gl_Context = SDL_GL_CreateContext(window);
	if (gl_Context == nullptr)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateContext Failed", SDL_GetError(), NULL);

		SDL_DestroyWindow(window);
		SDL_Quit();

		return 1;
	}

	//init GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		//Show error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "GLEW Initialisation Failed", (char*)glewGetErrorString(err), NULL);
		SDL_DestroyWindow(window);
		SDL_Quit();

		return 1;
	}

	std::vector<GameObject*> GameObjectList;

	glEnable(GL_DEPTH_TEST);

	Vertex verts[] =
	{

		{ -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f }, 
		{ -0.5f, -0.5f, 0.5f,-0.5f, -0.5f, 0.5f, 1.0f },
		{ 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 1.0f },
		{ 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f }, 

		{ -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 1.0f },
		{ -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f },
		{ 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 1.0f }
	};

	//Define triangles in the cube
	unsigned int indices[] =
	{
		1, 0 , 4,
		4, 0, 5,

		7, 2, 1,
		7, 1, 4,

		1, 2, 3,
		1, 3, 0,

		5, 0, 3,
		5, 3, 6,

		6, 3, 7,
		7, 3, 2,

		7, 4, 5,
		7, 5, 6
	};

	Mesh * morphMesh = new Mesh();
	morphMesh->init();
	morphMesh->copyBufferData(verts, 8, indices, 36);
	MeshCollection* morphMeshes = new MeshCollection();
	morphMeshes->addMesh(morphMesh);

	// Create and compile our GLSL program from the shader
	Shader * morphShader = new Shader();
	morphShader->Load("morphTargetVert.glsl", "frag.glsl");

	GameObject * cubeGO = new GameObject();
	cubeGO->SetPosition(0.0f, 0.0f, -10.0f);
	cubeGO->SetMesh(morphMeshes);
	cubeGO->SetShader(morphShader);




	MeshCollection * teapotMeshes = new MeshCollection();
	loadMeshFromFile("utah-teapot.fbx", teapotMeshes);

	Shader * texturedShader = new Shader();
	texturedShader->Load("texturedVert.glsl", "texturedFrag.glsl");

	GLuint textureID = loadTextureFromFile("brick_D.png");

	GameObject * teapotGO = new GameObject();
	teapotGO->SetPosition(0.0f, 0.0f, -50.0f);
	teapotGO->SetMesh(teapotMeshes);
	teapotGO->SetShader(texturedShader);
	teapotGO->SetDiffuseTexture(textureID);

	GameObjectList.push_back(cubeGO);
	GameObjectList.push_back(teapotGO);

	//Set up vectors for our camera position
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 10.0f);
	glm::vec3 cameraLook = glm::vec3(0.0f, 0.0f, -10.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	//Calculate the view matrix
	glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraLook, cameraUp);
	//Calculate our perspective matrix
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)800 / (float)640, 0.1f, 1000.0f);

	//Light properties
	glm::vec4 ambientLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec4 diffuseLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 specularLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightDirection = glm::vec3(0.0f, 0.0f, 1.0f);


	//Material Properties
	glm::vec4 ambientMaterialColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 diffuseMaterialColour = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	glm::vec4 specularMaterialColour= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float specularMaterialPower = 25.0f;

	//Point light
	std::vector<PointLight> PointLights;
	PointLights.push_back({ glm::vec4(1.0f,1.0f,1.0f,1.0f),glm::vec4(1.0f,1.0f,1.0f,1.0f),glm::vec3(-5.0f,0.0f,-40.0f) });
	PointLights.push_back({ glm::vec4(1.0f,1.0f,1.0f,1.0f),glm::vec4(1.0f,1.0f,1.0f,1.0f),glm::vec3(5.0f, 0.0f,-40.0f) });
	PointLights.push_back({ glm::vec4(1.0f,1.0f,1.0f,1.0f),glm::vec4(1.0f,1.0f,1.0f,1.0f),glm::vec3(5.0f,8.0f,-40.0f) });

	float morphBlendAlpha = 0.0f;

	GLuint colourBufferID = CreateTexture(800, 640);

	GLuint depthBufferID = 0;
	glGenRenderbuffers(1, &depthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 800, 640);

	GLuint framebufferID = 0;
	glGenFramebuffers(1, &framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colourBufferID, 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Frame Buffer", "Unable to create framebuffer", NULL);
	}

	float screenVerts[]=
	{
		-1,-1,
		1,-1,
		-1,1,
		1,1
	};

	GLuint screenQuadVB = 0;
	glGenBuffers(1, &screenQuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVB);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), screenVerts, GL_STATIC_DRAW);

	GLuint screenVAO = 0;
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVB);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	Shader * postProcessShader = new Shader();
	postProcessShader->Load("passThruVert.glsl", "postFrag.glsl");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Timer timer;
	timer.Start();

	bool startMorph = false;
	//Event loop, we will loop until running is set to false, usually if escape has been pressed or window is closed
	bool running = true;
	//SDL Event structure, this will be checked in the while loop
	SDL_Event ev;
	while (running)
	{
		//Poll for the events which have happened in this frame
		//https://wiki.libsdl.org/SDL_PollEvent
		while (SDL_PollEvent(&ev))
		{
			//Switch case for every message we are intereted in
			switch (ev.type)
			{
				//QUIT Message, usually called when the window has been closed
			case SDL_QUIT:
				running = false;
				break;
				//KEYDOWN Message, called when a key has been pressed down
			case SDL_KEYDOWN:
				//Check the actual key code of the key that has been pressed
				switch (ev.key.keysym.sym)
				{
					//Escape key
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_UP:
					morphBlendAlpha += 0.1f;
					morphBlendAlpha = glm::clamp(morphBlendAlpha, 0.0f, 1.0f);
					break;
				case SDLK_DOWN:
					morphBlendAlpha -= 0.1f;
					morphBlendAlpha = glm::clamp(morphBlendAlpha, 0.0f, 1.0f);
					break;
				case SDLK_SPACE:
					startMorph = true;
					morphBlendAlpha = 0.0f;
				}
			}
		}

		timer.Update();

		if (startMorph)
		{
			morphBlendAlpha += timer.GetDeltaTime();
			if (morphBlendAlpha>1.0f)
			{
				startMorph = false;
				morphBlendAlpha = 1.0f;
			}
		}

		//update
		for (GameObject * obj : GameObjectList)
		{
			obj->Update(timer.GetDeltaTime());
		}

		//Do rendering here!
		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (GameObject * obj : GameObjectList) {

			Shader * currentShader = obj->GetShader();
			currentShader->Use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,obj->GetDiffuseTexture());

			glUniformMatrix4fv(currentShader->GetUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(obj->GetModelTransformation()));
			glUniformMatrix4fv(currentShader->GetUniform("viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
			glUniformMatrix4fv(currentShader->GetUniform("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
			glUniform1f(currentShader->GetUniform("morphBlendAlpha"), morphBlendAlpha);
			glUniform1i(currentShader->GetUniform("diffuseTexture"), 0);


			obj->Render();
		}

		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		postProcessShader->Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colourBufferID);

		glUniform1i(postProcessShader->GetUniform("texture"), 0);

		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		SDL_GL_SwapWindow(window);
	}

	auto iter = GameObjectList.begin();
	while (iter != GameObjectList.end())
	{
		if ((*iter))
		{
			delete (*iter);
			iter = GameObjectList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	if (postProcessShader)
	{
		delete postProcessShader;
	}
	glDeleteVertexArrays(1, &screenVAO);
	glDeleteBuffers(1, &screenQuadVB);
	glDeleteFramebuffers(1, &framebufferID);
	glDeleteRenderbuffers(1, &depthBufferID);
	glDeleteTextures(1, &colourBufferID);
	GameObjectList.clear();
	//delete cubeGO;
	//Delete Context
	SDL_GL_DeleteContext(gl_Context);
	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(window);
	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();

	return 0;
}