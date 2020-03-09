#version 330 core

out vec4 colour;

in vec4 vertexColourOut;

//Materials
uniform vec4 ambientMaterialColour;

//Ambient Light Colour
uniform vec4 ambientLightColour;

void main()
{
  colour = ambientLightColour*ambientMaterialColour;
}