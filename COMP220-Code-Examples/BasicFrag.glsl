#version 330 core

out vec4 colour;

in vec4 vertexColourOut;

void main()
{
  colour = vertexColourOut;
}