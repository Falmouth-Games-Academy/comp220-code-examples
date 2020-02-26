#version 330 core

in vec2 vertexTexCoordOut;

uniform sampler2D baseTexture;

out vec4 color;

void main()
{
  color = texture(baseTexture,vertexTexCoordOut);
}