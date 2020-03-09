#version 330 core

struct DirectionalLight
{
	vec4 diffuseColour;
	vec4 specularColour;
	vec3 direction;
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


//Ambient Light Colour
uniform vec4 ambientLightColour;

uniform DirectionalLight directionalLight;

uniform Material material;


void main()
{
    //Lambert Diffuse
    float nDotl=clamp(dot(vertexNormalsOut,normalize(directionalLight.direction)),0.0,1.0);

    //Blinn Phong Specular
    vec3 halfWay=normalize(directionalLight.direction+viewDirection);
    float nDoth=pow(clamp(dot(vertexNormalsOut,halfWay),0.0,1.0),material.specularPower);
    
    colour = (ambientLightColour*material.ambientColour)+(directionalLight.diffuseColour*nDotl*material.diffuseColour)
                +(directionalLight.specularColour*nDoth*material.specularColour);
}