#version 330 core

in vec4 vertexColourOut;
in vec2 vertexTextureCoordOut;

out vec4 colour;

uniform sampler2D baseTexture;

uniform vec4 ambientLightColour;

uniform vec4 ambientMaterialColour;

void main()
{
	colour=ambientLightColour*ambientMaterialColour;
}