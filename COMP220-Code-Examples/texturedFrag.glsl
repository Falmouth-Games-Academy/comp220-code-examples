#version 330 core

in vec4 vertexColourOut;
in vec2 vertexTextureCoordOut;

out vec4 colour;

uniform sampler2D baseTexture;

void main()
{
	colour=texture2D(baseTexture,vertexTextureCoordOut);
}