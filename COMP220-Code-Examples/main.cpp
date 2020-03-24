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
#include "Texture.h"

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


	//Post processing setup
	GLuint postTextureID = CreateTexture(1280, 720);

	GLuint depthBufferID;
	glGenRenderbuffers(1, &depthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720);

	GLuint frameBufferID;
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, postTextureID, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		//error message
	}

	//Create quad to support post processing
	float vertices[] =
	{
		-1,	-1,
		1,	-1,
		-1,  1,
		1,   1,
	};

	GLuint screenVBO;
	glGenBuffers(1, &screenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_STATIC_DRAW);

	GLuint screenVAO;
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint postProcessingProgramID = LoadShaders("passThruVert.glsl", "postFrag.glsl");
	GLuint texxture0Location = glGetUniformLocation(postProcessingProgramID,"texture0");



	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("ParallaxMappingVert.glsl", 
		"ParallaxMappingFrag.glsl");

	//**** Begin Model Loading Example 
	MeshCollection* currentMeshes=new MeshCollection();
	loadMeshCollectionFromFile("2019-20-utah-teapot.fbx", currentMeshes);
	//**** End Model Loading Example

	//Get Uniform locations - Matrices
	GLuint modelLocation = glGetUniformLocation(programID, "model");
	GLuint viewLocation = glGetUniformLocation(programID, "view");
	GLuint projectionLocation = glGetUniformLocation(programID, "projection");
	
	//Get Uniform locations - Ambient Light
	GLuint ambientLightColourLocation = glGetUniformLocation(programID, "ambientLightColour");

	//Get Uniform locations - Directional Light
	GLuint diffuseDirectionalLightColourLocation = glGetUniformLocation(programID, "directionalLight.diffuseColour");
	GLuint specularDirectionalLightColourLocation = glGetUniformLocation(programID, "directionalLight.specularColour");
	GLuint lightDirectionLocation = glGetUniformLocation(programID, "directionalLight.direction");

	////Get Uniform locations - Point Light
	GLuint diffusePointLightColourLocation = glGetUniformLocation(programID, "pointLight.diffuseColour");
	GLuint specularPointLightColourLocation = glGetUniformLocation(programID, "pointLight.specularColour");
	GLuint pointLightPositionLocation = glGetUniformLocation(programID, "pointLight.position");

	//Get Uniform locations - Materials
	GLuint ambientMaterialColourLocation = glGetUniformLocation(programID, "material.ambientColour");
	GLuint diffuseMaterialColourLocation = glGetUniformLocation(programID, "material.diffuseColour");
	GLuint specularMaterialColourLocation = glGetUniformLocation(programID, "material.specularColour");
	GLuint specularMaterialPowerLocation = glGetUniformLocation(programID, "material.specularPower");

	//Get Uniform locations - Camera
	GLuint cameraPositionLocation = glGetUniformLocation(programID, "cameraPosition");

	//Get Uniform locations - Textures
	GLuint albedoTextureLocation = glGetUniformLocation(programID, "albedoTexture");
	GLuint specTextureLocation = glGetUniformLocation(programID, "specTexture");
	GLuint normalTextureLocation = glGetUniformLocation(programID, "normalTexture");
	GLuint heightTextureLocation = glGetUniformLocation(programID, "heightTexture");

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
	glm::vec4 diffuseDirectionalLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 specularDirectionalLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);


	//Point Light
	glm::vec3 lightPosition=glm::vec3(0.0f, 5.0f, -30.0f);
	glm::vec4 diffusePointLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 specularPointLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);


	//Materials
	glm::vec4 ambientMaterialColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 diffuseMaterialColour = glm::vec4(0.519f, 0.848f, 0.746f,1.0f );
	glm::vec4 specularMaterialColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float specularMaterialPower = 50.0f;

	//Load and assign the textures to slots, make note of the
	//number for sending to shader
	glActiveTexture(GL_TEXTURE0);
	GLuint albedoTextureID = loadTextureFromFile("brick_D.png");

	glActiveTexture(GL_TEXTURE1);
	GLuint specTextureID = loadTextureFromFile("spot_S.png");

	glActiveTexture(GL_TEXTURE2);
	GLuint normalTextureID = loadTextureFromFile("brick_N.png");

	glActiveTexture(GL_TEXTURE3);
	GLuint heightTextureID = loadTextureFromFile("brick_H.png");

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

		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
		glClearDepth(1.0f);

		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		//Bind a shader program
		glUseProgram(programID);

		//Send Matrices
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		//Send Ambient Light Colour
		glUniform4fv(ambientLightColourLocation, 1, glm::value_ptr(ambientLightColour));

		glUniform4fv(diffuseDirectionalLightColourLocation, 1, glm::value_ptr(diffuseDirectionalLightColour));
		glUniform4fv(specularDirectionalLightColourLocation, 1, glm::value_ptr(specularDirectionalLightColour));
		glUniform3fv(lightDirectionLocation, 1, glm::value_ptr(lightDirection));


		glUniform4fv(diffusePointLightColourLocation, 1, glm::value_ptr(diffusePointLightColour));
		glUniform4fv(specularPointLightColourLocation, 1, glm::value_ptr(specularPointLightColour));
		glUniform3fv(pointLightPositionLocation, 1, glm::value_ptr(lightPosition));

		glUniform4fv(ambientMaterialColourLocation, 1, glm::value_ptr(ambientMaterialColour));
		glUniform4fv(diffuseMaterialColourLocation, 1, glm::value_ptr(diffuseMaterialColour));
		glUniform4fv(specularMaterialColourLocation, 1, glm::value_ptr(specularMaterialColour));
		glUniform1f(specularMaterialPowerLocation, specularMaterialPower);
		
		glUniform3fv(cameraPositionLocation, 1, glm::value_ptr(cameraPosition));

		glUniform1i(albedoTextureLocation, 0);
		glUniform1i(specTextureLocation, 1);
		glUniform1i(normalTextureLocation, 2);
		glUniform1i(heightTextureLocation, 3);

		currentMeshes->render();

		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(postProcessingProgramID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, postTextureID);
		glUniform1d(texxture0Location, 0);

		//draw quad
		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);



		SDL_GL_SwapWindow(window);
	}

	currentMeshes->destroy();
	if (currentMeshes)
	{
		delete currentMeshes;
	}

	glDeleteTextures(1, &albedoTextureID);

	glDeleteProgram(programID);

	glDeleteBuffers(1, &screenVBO);
	glDeleteVertexArrays(1, &screenVAO);

	glDeleteProgram(postProcessingProgramID);

	glDeleteRenderbuffers(1, &depthBufferID);
	glDeleteTextures(1, &postTextureID);
	glDeleteFramebuffers(1, &frameBufferID);
	SDL_GL_DeleteContext(glContext);
	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(window);
	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();

	return 0;
}