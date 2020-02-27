#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColourOut;

void main()
{
	mat4 mvpMatrix=projection*view*model;

	vertexColourOut=vertexColour;

	gl_Position = mvpMatrix*vec4(vertexPosition,1.0f);
}