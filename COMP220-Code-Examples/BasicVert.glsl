#version 330 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{	
	mat4 mvpMatrix=projection*view*model;
	gl_Position = mvpMatrix*vec4(vertexPosition,1.0f);
}