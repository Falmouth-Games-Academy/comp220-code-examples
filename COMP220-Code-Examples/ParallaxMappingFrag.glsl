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
};

struct Material
{
    vec4 ambientColour;
    vec4 diffuseColour;
    vec4 specularColour;
    float specularPower;
};


out vec4 colour;

in vec4 vertexColourOut;
in vec2 vertexTextureCoordOut;
in vec3 vertexNormalsOut;
in vec3 viewDirection;
in vec3 worldVertexPosition;
in mat3 tangentMatrix;

//Ambient Light Colour
uniform vec4 ambientLightColour;

uniform DirectionalLight directionalLight;

uniform PointLight pointLight;

uniform Material material;

uniform sampler2D albedoTexture;
uniform sampler2D specTexture;
uniform sampler2D normalTexture;
uniform sampler2D heightTexture;

const float heightScale=0.1f;

vec4 CalculateLightColour(vec4 diffuseLightColour,vec4 specularLightColour,vec3 lightDirection)
{
    vec3 tangentLight=tangentMatrix*lightDirection;
    vec3 tangentView=tangentMatrix*viewDirection;

    float height=texture(heightTexture, vertexTextureCoordOut).x;
    vec2 heightOffset=viewDirection.xy/viewDirection.z*(height*heightScale);
    vec2 displacedTextureCoords=vertexTextureCoordOut-heightOffset;

	vec3 normal=texture(normalTexture,displacedTextureCoords).xyz;
	normal=normalize(normal * 2.0 - 1.0);
	normal=normalize(tangentMatrix*normal);

    //Lambert Diffuse
    float nDotl=clamp(dot(normal,normalize(tangentLight)),0.0,1.0);

    //Blinn Phong Specular
    vec3 halfWay=normalize(tangentLight+tangentView);
    float nDoth=pow(clamp(dot(normal,halfWay),0.0,1.0),material.specularPower);

    vec4 albedo = texture(albedoTexture,displacedTextureCoords);
    vec4 specTextureColour=texture(specTexture,displacedTextureCoords);

    return (diffuseLightColour*nDotl*material.diffuseColour*albedo)
                +(specularLightColour*nDoth*material.specularColour*specTextureColour);
}

vec4 CalculateDirectionLightColour()
{
	return CalculateLightColour(directionalLight.diffuseColour,
								directionalLight.specularColour,
								directionalLight.direction);
}

vec4 CalculuatePointLight()
{
    vec3 lightDirection=worldVertexPosition-pointLight.position;
	float lightDistance=length(lightDirection);
    lightDirection=normalize(lightDirection);
    
    vec4 colour=CalculateLightColour(pointLight.diffuseColour,pointLight.specularColour,lightDirection);

    float attenuation=1.0/(1.0+0.1*lightDistance+0.01*lightDistance*lightDistance);
    
    return colour*attenuation;
}

void main()
{
    //Get final colour from lighting functions
    vec4 finalColour=(ambientLightColour*material.ambientColour)+CalculateDirectionLightColour()+CalculuatePointLight();
    colour=finalColour;
}