#version 330 core
//More examples here
//https://medium.com/@joshmarinacci/water-ripples-with-vertex-shaders-6a9ecbdf091f
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexTarget;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float morphBlendAlpha;

out vec4 vertexColoursOut;

void main(){
	
	mat4 mvpMatrix=projectionMatrix*viewMatrix*modelMatrix;

	vec3 adjustedVertexPosition=mix(vertexPosition,vertexTarget.xyz,clamp(morphBlendAlpha,0.0f,1.0f));

	vec4 worldPosition=modelMatrix*vec4(adjustedVertexPosition,1.0f);
	vec4 mvpPosition=mvpMatrix*vec4(adjustedVertexPosition,1.0f);

	//worldPosition.y+=(sin(currentTime+worldPosition.x)*3.0)*0.1;
	
	vertexColoursOut=vec4(1.0f);
	gl_Position=mvpPosition;
}