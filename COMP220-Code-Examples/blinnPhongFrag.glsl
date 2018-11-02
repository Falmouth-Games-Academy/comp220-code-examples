#version 330 core

struct DirectionalLight
{
	vec4 diffuseColour;
	vec4 specularColour;
	vec3 direction;
};

struct PointLight
{
	vec4 diffuseColour;
	vec4 specularColour;
	vec3 position;

	float constantAttenuation;
	float linerAttenuation;
	float exponetialAttenuation;
};

const int MAX_NO_OF_POINT_LIGHTS=8;

in vec4 vertexColourOut;
in vec2 vertexTextureCoordOut;
in vec3 vertexNormalsOut;
in vec3 viewDirection;
in vec3 worldVertexPosition;

out vec4 colour;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform vec4 ambientMaterialColour;
uniform vec4 diffuseMaterialColour;
uniform vec4 specularMaterialColour;
uniform float specularMaterialPower;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_NO_OF_POINT_LIGHTS];
uniform int NumberOfPointLights;

uniform vec4 ambientLightColour;


vec4 CalculateLightColour(vec4 diffuseLightColour,vec4 specularLightColour,vec3 lightDirection,vec4 diffuseTextureColour,vec4 specularTextureColour)
{
	float nDotl=dot(vertexNormalsOut,normalize(lightDirection));

	vec3 halfWay=normalize(lightDirection+viewDirection);
	float nDoth=pow(dot(vertexNormalsOut,halfWay),specularMaterialPower);

	return 	(diffuseLightColour*nDotl*diffuseMaterialColour*diffuseTextureColour)+
	(specularLightColour*nDoth*specularMaterialColour*specularTextureColour);
}

vec4 CalculatePointLight(int currentLightIndex,vec4 diffuseTextureColour,vec4 specularTextureColour)
{
	vec3 lightDirection=worldVertexPosition-pointLights[currentLightIndex].position;
	float lightDistance=length(lightDirection);
	lightDirection=normalize(lightDirection);

	vec4 colour=CalculateLightColour(pointLights[currentLightIndex].diffuseColour,pointLights[currentLightIndex].specularColour,lightDirection,diffuseTextureColour,specularTextureColour);
	
	float attenuation=pointLights[currentLightIndex].constantAttenuation +
                       pointLights[currentLightIndex].linerAttenuation * lightDistance +
                        pointLights[currentLightIndex].exponetialAttenuation * lightDistance * lightDistance;

	return colour/attenuation;
}

vec4 CalculateDirectionLightColour(vec4 diffuseTextureColour,vec4 specularTextureColour)
{
	return CalculateLightColour(directionalLight.diffuseColour,directionalLight.specularColour,directionalLight.direction,diffuseTextureColour,specularTextureColour);
}


void main()
{
	vec4 diffuseTextureColour=texture(diffuseTexture,vertexTextureCoordOut);

	vec4 specularTextureColour=texture(specularTexture,vertexTextureCoordOut);

	vec4 finalColour=CalculateDirectionLightColour(diffuseTextureColour,specularTextureColour);

	for (int i=0;i<NumberOfPointLights;i++)
	{
		finalColour+=CalculatePointLight(i,diffuseTextureColour,specularTextureColour);
	}

	colour=(ambientLightColour*ambientMaterialColour)+finalColour;
}