#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColour;
layout(location = 2) in vec2 vertexTexCoord;


out vec2 vertexTexCoordOut;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{	
	mat4 mvpMatrix=projection*view*model;

	vertexTexCoordOut=vertexTexCoord;
	
	gl_Position = mvpMatrix*vec4(vertexPosition,1.0f);
}