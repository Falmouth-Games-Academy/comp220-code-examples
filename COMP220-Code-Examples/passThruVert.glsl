#version 330 core
layout(location=0) in vec2 vertexPosition;

out vec2 textureCoords;

void main()
{
	//Calculate Texture Coordinates for the Vertex
	textureCoords = (vertexPosition + 1.0) / 2.0;
	gl_Position = vec4(vertexPosition, 0.0, 1.0);
}