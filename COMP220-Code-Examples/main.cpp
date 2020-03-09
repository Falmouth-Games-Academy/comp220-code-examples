#include <iostream>
#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"
#include "Model.h"
#include "Vertex.h"

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

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Create a window, note we have to free the pointer returned using the DestroyWindow Function
	//https://wiki.libsdl.org/SDL_CreateWindow
	SDL_Window* window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
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

	SDL_GLContext glContext = SDL_GL_CreateContext(window);



	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Unable to initialise GLEW", (char*)glewGetErrorString(glewError), NULL);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("LightingVert.glsl", 
		"lightingFrag.glsl");

	//**** Begin Model Loading Example 
	MeshCollection* currentMeshes=new MeshCollection();
	loadMeshCollectionFromFile("2019-20-utah-teapot.fbx", currentMeshes);
	//**** End Model Loading Example

	//Get Uniform locations
	GLuint modelLocation = glGetUniformLocation(programID, "model");
	GLuint viewLocation = glGetUniformLocation(programID, "view");
	GLuint projectionLocation = glGetUniformLocation(programID, "projection");
	
	GLuint ambientLightColourLocation = glGetUniformLocation(programID, "ambientLightColour");
	GLuint diffuseLightColourLocation = glGetUniformLocation(programID, "directionalLight.diffuseColour");
	GLuint specularLightColourLocation = glGetUniformLocation(programID, "directionalLight.specularColour");
	GLuint lightDirectionLocation = glGetUniformLocation(programID, "directionalLight.direction");

	GLuint ambientMaterialColourLocation = glGetUniformLocation(programID, "material.ambientColour");
	GLuint diffuseMaterialColourLocation = glGetUniformLocation(programID, "material.diffuseColour");
	GLuint specularMaterialColourLocation = glGetUniformLocation(programID, "material.specularColour");
	GLuint specularMaterialPowerLocation = glGetUniformLocation(programID, "material.specularPower");

	GLuint cameraPositionLocation = glGetUniformLocation(programID, "cameraPosition");

	//Set up vectors for our camera position
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 20.0f);
	glm::vec3 cameraLook = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	//Calculate the view matrix
	glm::mat4 view = glm::lookAt(cameraPosition, cameraLook, cameraUp);
	//Calculate our perspective matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 1000.0f);

	glm::vec3 position = glm::vec3(0.0f, -5.0f, -50.0f);
	glm::mat4 model = glm::translate(position);

	//Ambient Light Colour
	glm::vec4 ambientLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	//Directional Light
	glm::vec3 lightDirection = glm::vec3(0.0f, -0.5f, 0.5f);

	glm::vec4 diffuseLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 specularLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	//Materials
	glm::vec4 ambientMaterialColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 diffuseMaterialColour = glm::vec4(0.54f, 0.80f, 0.43f, 1.0f);
	glm::vec4 specularMaterialColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float specularMaterialPower = 50.0f;


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
				}
			}
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
		glClearDepth(1.0f);

		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform4fv(ambientLightColourLocation, 1, glm::value_ptr(ambientLightColour));
		glUniform4fv(diffuseLightColourLocation, 1, glm::value_ptr(diffuseLightColour));
		glUniform4fv(specularLightColourLocation, 1, glm::value_ptr(specularLightColour));
		glUniform3fv(lightDirectionLocation, 1, glm::value_ptr(lightDirection));
		glUniform4fv(ambientMaterialColourLocation, 1, glm::value_ptr(ambientMaterialColour));
		glUniform4fv(diffuseMaterialColourLocation, 1, glm::value_ptr(diffuseMaterialColour));
		glUniform4fv(specularMaterialColourLocation, 1, glm::value_ptr(specularMaterialColour));
		glUniform1f(specularMaterialPowerLocation, specularMaterialPower);
		glUniform3fv(cameraPositionLocation, 1, glm::value_ptr(cameraPosition));


		currentMeshes->render();

		SDL_GL_SwapWindow(window);
	}

	currentMeshes->destroy();
	if (currentMeshes)
	{
		delete currentMeshes;
	}

	glDeleteProgram(programID);
	SDL_GL_DeleteContext(glContext);
	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(window);
	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();

	return 0;
}