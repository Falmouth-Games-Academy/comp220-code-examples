#version 330 core

in vec4 vertexColourOut;
in vec2 vertexTextureCoordOut;
in vec3 vertexNormalsOut;

out vec4 colour;

uniform sampler2D baseTexture;

uniform vec4 ambientLightColour;
uniform vec4 diffuseLightColour;

uniform vec3 lightDirection;

uniform vec4 ambientMaterialColour;
uniform vec4 diffuseMaterialColour;

void main()
{
	float nDotl=dot(vertexNormalsOut,normalize(lightDirection));
	
	colour=(ambientLightColour*ambientMaterialColour)+(diffuseLightColour*nDotl*diffuseMaterialColour);
}