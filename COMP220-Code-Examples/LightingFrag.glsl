#version 330 core

out vec4 colour;

in vec4 vertexColourOut;
in vec2 vertexTextureCoordOut;
in vec3 vertexNormalsOut;

//Materials
uniform vec4 ambientMaterialColour;
uniform vec4 diffuseMaterialColour;

//Ambient Light Colour
uniform vec4 ambientLightColour;

//Light Direction
uniform vec3 lightDirection;
uniform vec4 diffuseLightColour;

void main()
{
    float nDotl=clamp(dot(vertexNormalsOut,normalize(lightDirection)),0.0,1.0);
    
    colour = (ambientLightColour*ambientMaterialColour)+(diffuseLightColour*nDotl*diffuseMaterialColour);
}