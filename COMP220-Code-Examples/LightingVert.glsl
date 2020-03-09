#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColour;
layout(location=2) in vec2 vertexTextureCoord;
layout(location=3) in vec3 vertexNormals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPosition;

out vec4 vertexColourOut;
out vec2 vertexTextureCoordOut;
out vec3 vertexNormalsOut;
out vec3 viewDirection;
out vec3 worldVertexPosition;

void main()
{
	mat4 mvpMatrix=projection*view*model;

	vec4 worldPosition=model*vec4(vertexPosition,1.0f);

	vertexColourOut=vertexColour;
	vertexTextureCoordOut=vertexTextureCoord;
	vertexNormalsOut=normalize(model*vec4(vertexNormals,0.0f)).xyz;

	viewDirection=normalize(cameraPosition-worldPosition.xyz);
	worldVertexPosition=worldPosition.xyz;

	gl_Position = mvpMatrix*vec4(vertexPosition,1.0f);
}