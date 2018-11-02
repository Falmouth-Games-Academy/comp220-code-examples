#version 330 core

in vec4 vertexColourOut;
in vec2 vertexTextureCoordOut;
in vec3 vertexNormalsOut;
in vec3 viewDirection;

out vec4 colour;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform vec4 ambientMaterialColour;
uniform vec4 diffuseMaterialColour;
uniform vec4 specularMaterialColour;
uniform float specularMaterialPower;

struct DirectionalLight
{
	vec4 diffuseColour;
	vec4 specularColour;
	vec3 direction;
};

uniform DirectionalLight directionalLight;
uniform vec4 ambientLightColour;


void main()
{
	//Diffuse
	float nDotl=dot(vertexNormalsOut,normalize(directionalLight.direction));

	//Specular
	vec3 halfWay=normalize(directionalLight.direction+viewDirection);
	float nDoth=pow(dot(vertexNormalsOut,halfWay),specularMaterialPower);

	vec4 diffuseTextureColour=texture(diffuseTexture,vertexTextureCoordOut);

	vec4 specularTextureColour=texture(specularTexture,vertexTextureCoordOut);

	colour=(ambientLightColour*ambientMaterialColour)+
	(directionalLight.diffuseColour*nDotl*diffuseMaterialColour*diffuseTextureColour)+
	(directionalLight.specularColour*nDoth*specularMaterialColour*specularTextureColour);
}