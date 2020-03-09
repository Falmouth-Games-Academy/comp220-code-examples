#version 330 core

out vec4 colour;

in vec4 vertexColourOut;
in vec2 vertexTextureCoordOut;
in vec3 vertexNormalsOut;
in vec3 viewDirection;

//Materials
uniform vec4 ambientMaterialColour;
uniform vec4 diffuseMaterialColour;
uniform vec4 specularMaterialColour;
uniform float specularMaterialPower;

//Ambient Light Colour
uniform vec4 ambientLightColour;

//Light Direction
uniform vec3 lightDirection;
uniform vec4 diffuseLightColour;
uniform vec4 specularLightColour;

void main()
{
    //Lambert Diffuse
    float nDotl=clamp(dot(vertexNormalsOut,normalize(lightDirection)),0.0,1.0);

    //Blinn Phong Specular
    vec3 halfWay=normalize(lightDirection+viewDirection);
    float nDoth=pow(clamp(dot(vertexNormalsOut,halfWay),0.0,1.0),specularMaterialPower);
    
    colour = (ambientLightColour*ambientMaterialColour)+(diffuseLightColour*nDotl*diffuseMaterialColour)
                +(specularLightColour*nDoth*specularMaterialColour);
}