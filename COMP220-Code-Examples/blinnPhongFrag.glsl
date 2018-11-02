#version 330 core

in vec4 vertexColourOut;
in vec2 vertexTextureCoordOut;
in vec3 vertexNormalsOut;
in vec3 viewDirection;

out vec4 colour;

uniform sampler2D baseTexture;

uniform vec4 ambientLightColour;
uniform vec4 diffuseLightColour;
uniform vec4 specularLightColour;

uniform vec3 lightDirection;

uniform vec4 ambientMaterialColour;
uniform vec4 diffuseMaterialColour;
uniform vec4 specularMaterialColour;
uniform float specularMaterialPower;

void main()
{
	//Diffuse
	float nDotl=dot(vertexNormalsOut,normalize(lightDirection));

	//Specular
	vec3 halfWay=normalize(lightDirection+viewDirection);
	float nDoth=pow(dot(vertexNormalsOut,halfWay),specularMaterialPower);

	
	colour=(ambientLightColour*ambientMaterialColour)+(diffuseLightColour*nDotl*diffuseMaterialColour)+(specularLightColour*nDoth*specularMaterialColour);
}