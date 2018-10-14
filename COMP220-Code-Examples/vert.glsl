#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColours;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 vertexColoursOut;

void main(){
	
	mat4 mvpMatrix=projectionMatrix*viewMatrix*modelMatrix;

	vec4 mvpPosition=mvpMatrix*vec4(vertexPosition,1.0f);
	
	vertexColoursOut=vertexColours;

	gl_Position=mvpPosition;
}