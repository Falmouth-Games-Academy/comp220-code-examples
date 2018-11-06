#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColours;
layout(location=2) in vec2 vertexTextureCoord;
layout(location=3) in vec3 vertexNormals;
layout(location=4) in vec3 vertexTangents;
layout(location=5) in vec3 vertexBitangents;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 cameraPosition;

out vec4 vertexColoursOut;
out vec2 vertexTextureCoordOut;
out vec3 vertexNormalsOut;
out vec3 viewDirection;
out vec3 worldVertexPosition;
out mat3 tangentMatrix;

void main(){
	
	mat4 mvpMatrix=projectionMatrix*viewMatrix*modelMatrix;

	vec4 worldPosition=modelMatrix*vec4(vertexPosition,1.0f);
	vec4 mvpPosition=mvpMatrix*vec4(vertexPosition,1.0f);
	
	vec3 worldTangent=(normalize(modelMatrix*vec4(vertexTangents,0.0f))).xyz;
	vec3 worldBinormal=(normalize(modelMatrix*vec4(vertexBitangents,0.0f))).xyz;
	vec3 worldNormal=normalize(modelMatrix*vec4(vertexNormals,0.0f)).xyz;
	tangentMatrix=mat3(worldTangent,worldBinormal,worldNormal);

	vertexColoursOut=vertexColours;
	vertexTextureCoordOut=vertexTextureCoord;
	vertexNormalsOut=worldNormal;
	viewDirection=normalize(cameraPosition-worldPosition.xyz);
	worldVertexPosition=worldPosition.xyz;

	gl_Position=mvpPosition;
}