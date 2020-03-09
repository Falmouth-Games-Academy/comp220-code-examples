#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColour;
layout(location=2) in vec2 vertexTextureCoord;
layout(location=3) in vec3 vertexNormals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColourOut;
out vec2 vertexTextureCoordOut;
out vec3 vertexNormalsOut;

void main()
{
	mat4 mvpMatrix=projection*view*model;

	vertexColourOut=vertexColour;
	vertexTextureCoordOut=vertexTextureCoord;
	vertexNormalsOut=normalize(model*vec4(vertexNormals,0.0f)).xyz;

	gl_Position = mvpMatrix*vec4(vertexPosition,1.0f);
}