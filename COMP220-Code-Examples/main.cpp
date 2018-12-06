#include <iostream>
#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <btBulletDynamicsCommon.h>

#include "Shaders.h"
#include "Vertex.h"
#include "Texture.h"
#include "Model.h"
#include "Mesh.h"
#include "Light.h"
#include "Timer.h"
#include "GameObject.h"
#include "OpenGLBulletDebugDrawer.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplSDL2_InitForOpenGL(window, gl_Context);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Setup Style
	ImGui::StyleColorsDark();

	std::vector<GameObject*> GameObjectList;

	glEnable(GL_DEPTH_TEST);


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

	//init physics

	//Create Config for Collision
	btDefaultCollisionConfiguration* collisionConfig = new btDefaultCollisionConfiguration();

	//Collision Dispatcher
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);

	//Create Broadphase
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	//solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	//Create Physics World
	btDiscreteDynamicsWorld* dynamicWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfig);

	OpenGLBulletDebugDrawer debugDrawer;
	debugDrawer.CreateShader();
	
	dynamicWorld->setDebugDrawer(&debugDrawer);


	MeshCollection * boxMeshes = new MeshCollection();
	loadMeshFromFile("cube.nff", boxMeshes);

	Shader * texturedShader = new Shader();
	texturedShader->Load("texturedVert.glsl", "texturedFrag.glsl");

	GLuint textureID = loadTextureFromFile("brick_D.png");

	GameObject * groundGO = new GameObject();
	groundGO->SetName("Ground");
	groundGO->SetPosition(0.0f, -10.0f, -120.0f);
	groundGO->SetScale(100.0f, 1.0f, 100.0f);
	groundGO->SetMesh(boxMeshes);
	groundGO->SetShader(texturedShader);
	groundGO->SetDiffuseTexture(textureID);

	GameObjectList.push_back(groundGO);

	//Create ground shadpe
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.0), btScalar(0.5), btScalar(50.0)));

	btTransform groundTransform;
	groundTransform.setIdentity();
	glm::vec3 groundPosition = groundGO->GetPosition();
	groundTransform.setOrigin(btVector3(groundPosition.x, groundPosition.y, groundPosition.z));

	//Create Ground Rigid Body
	btVector3 groundLocalInertia = btVector3(0, 0, 0);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo groundInfo(btScalar(0.0), groundMotionState, groundShape, groundLocalInertia);
	btRigidBody* groundBody = new btRigidBody(groundInfo);

	dynamicWorld->addRigidBody(groundBody);
	groundBody->setUserPointer(groundGO);
	groundGO->SetRigidBody(groundBody);

	float startY = 40.0f;
	for (int i = 0; i < 5; i++) {
		MeshCollection * sphereMeshes = new MeshCollection();
		loadMeshFromFile("sphere.nff", sphereMeshes);

		Shader * standardShader = new Shader();
		standardShader->Load("vert.glsl", "frag.glsl");

		GameObject * sphereGO = new GameObject();
		sphereGO->SetName("Sphere");
		sphereGO->SetPosition(0.0f, startY, -120.0f);
		sphereGO->SetMesh(sphereMeshes);
		sphereGO->SetShader(standardShader);

		GameObjectList.push_back(sphereGO);


		//Create Sphere Shape
		btCollisionShape* sphereShape = new btSphereShape(btScalar(1.0));
		btTransform sphereTransform;
		sphereTransform.setIdentity();
		glm::vec3 spherePosition = sphereGO->GetPosition();

		btScalar sphereMass = 1.0;
		btVector3 sphereLocalInertia = btVector3(0, 0, 0);
		sphereShape->calculateLocalInertia(sphereMass, sphereLocalInertia);

		sphereTransform.setOrigin(btVector3(spherePosition.x, spherePosition.y, spherePosition.z));

		//Rigid Body
		btMotionState* sphereMotionState = new btDefaultMotionState(sphereTransform);
		btRigidBody::btRigidBodyConstructionInfo sphereInfo(sphereMass, sphereMotionState, sphereShape, sphereLocalInertia);
		btRigidBody* sphereBody = new btRigidBody(sphereInfo);

		dynamicWorld->addRigidBody(sphereBody);
		sphereGO->SetRigidBody(sphereBody);
		sphereBody->setUserPointer(sphereGO);

		startY += 50.0f;
	}


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
			ImGui_ImplSDL2_ProcessEvent(&ev);
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
			case SDL_MOUSEBUTTONDOWN:
				if (ev.button.button == SDL_BUTTON_LEFT)
				{

					//http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-a-physics-library/
					//http://antongerdelan.net/opengl/raycasting.html

					int xMousePosition = ev.button.x;
					int yMousePosition = ev.button.y;

					float xMouseNDSPosition = ((2.0f*xMousePosition) / 800) - 1.0f;
					float yMouseNDSPosition = 1.0f - (2.0f*yMousePosition) / 640;
					
					glm::vec3 rayEndNDS = glm::vec3(xMouseNDSPosition, yMouseNDSPosition, 1.0f);
					glm::vec4 rayEndClip = glm::vec4(rayEndNDS.x, rayEndNDS.y, -1.0f, 1.0f);

					glm::vec4 rayEndView = glm::inverse(projectionMatrix)*rayEndClip;
					rayEndView = glm::vec4(rayEndView.x, rayEndView.y, -1.0f, 0.0f);

					glm::vec3 rayEndWorld = glm::vec3((glm::inverse(viewMatrix)*rayEndView));
					rayEndWorld = glm::normalize(rayEndWorld);

					glm::vec3 rayEndPosition = cameraPosition + (rayEndWorld*200.0f);

					btCollisionWorld::AllHitsRayResultCallback RayCallback(
						btVector3(cameraPosition.x, cameraPosition.y, cameraPosition.z),
						btVector3(rayEndPosition.x, rayEndPosition.y, rayEndPosition.z)
					);

					dynamicWorld->rayTest(
						btVector3(cameraPosition.x, cameraPosition.y, cameraPosition.z),
						btVector3(rayEndPosition.x, rayEndPosition.y, rayEndPosition.z),
						RayCallback
					);
					

					if (RayCallback.hasHit())
					{
						int numCollisions = RayCallback.m_collisionObjects.size();
						for (int i=0;i<numCollisions;i++)
						{
							//Raycallback.m_collisionObject
							GameObject * hitObject = (GameObject*)RayCallback.m_collisionObjects[i]->getUserPointer();
							std::cout << "Hit something " << hitObject->GetName() << std::endl;
							if (hitObject->GetName() == "Sphere")
							{
								hitObject->GetRigidBody()->applyCentralForce(btVector3(0.0f, 0.0f, -100.0f));
							}
						}
					}
					
				}
				break;
			}

		}

		timer.Update();

		dynamicWorld->stepSimulation(timer.GetDeltaTime(), 10);
		//Check for collisions
		int numCollisionManifolds = dynamicWorld->getDispatcher()->getNumManifolds();
		//iterate through these
		for (int i = 0; i < numCollisionManifolds; i++)
		{
			//Get the manifold
			btPersistentManifold* contactManifold = dynamicWorld->getDispatcher()->getManifoldByIndexInternal(i);
			
			//Check if we have a contact
			int numContacts = contactManifold->getNumContacts();
			if (numContacts>0)
			{
				//get the base collision objects
				const btCollisionObject* collisionObject0 = contactManifold->getBody0();
				const btCollisionObject* collisionObject1 = contactManifold->getBody1();

				//Grab the Game objects via the user poiner (see object creation above)
				GameObject * gameObject0 = (GameObject*)collisionObject0->getUserPointer();
				GameObject * gameObject1 = (GameObject*)collisionObject1->getUserPointer();

				if (gameObject0->GetName() == "Sphere")
				{
				}
				if (gameObject1->GetName() == "Sphere")
				{
				}

			}
		}
		


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

		debugDrawer.SetViewAndProjectMatrix(viewMatrix, projectionMatrix);
		dynamicWorld->debugDrawWorld();

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

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		ImGui::Begin("Hello, world!");
		ImGui::Text("This is some useful text.");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}


	auto iter = GameObjectList.begin();
	while (iter != GameObjectList.end())
	{
		if ((*iter))
		{
			btRigidBody * rb = (*iter)->GetRigidBody();
			dynamicWorld->removeRigidBody(rb);
			
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

	delete dynamicWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfig;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

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