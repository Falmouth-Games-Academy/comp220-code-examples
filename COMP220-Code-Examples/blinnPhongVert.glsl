#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColours;
layout(location=2) in vec2 vertexTextureCoord;
layout(location=3) in vec3 vertexNormals;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 cameraPosition;

out vec4 vertexColoursOut;
out vec2 vertexTextureCoordOut;
out vec3 vertexNormalsOut;
out vec3 viewDirection;

void main(){
	
	mat4 mvpMatrix=projectionMatrix*viewMatrix*modelMatrix;

	vec4 worldPosition=modelMatrix*vec4(vertexPosition,1.0f);
	vec4 mvpPosition=mvpMatrix*vec4(vertexPosition,1.0f);
	
	vertexColoursOut=vertexColours;
	vertexTextureCoordOut=vertexTextureCoord;
	vertexNormalsOut=normalize(modelMatrix*vec4(vertexNormals,0.0f)).xyz;
	viewDirection=normalize(cameraPosition-worldPosition.xyz);

	gl_Position=mvpPosition;
}