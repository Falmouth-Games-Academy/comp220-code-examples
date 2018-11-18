#version 330 core

layout(location = 0) in vec2 vertexPosition;

out vec2 vertexTextureCoordOut;

void main(){

	vertexTextureCoordOut=(vertexPosition+1.0f)/2.0f;
	gl_Position=vec4(vertexPosition,0.0f,1.0f);
}