#version 330 core

in vec4 vertexColourOut;
in vec2 vertexTextureCoordOut;

out vec4 colour;

uniform sampler2D texture;

float getLuminance(vec3 colour)
{
	return (colour.r+colour.g+colour.b)/3.0f;
}

void main()
{
	vec4 textureColour=texture2D(texture,vertexTextureCoordOut);
	float luminance=getLuminance(textureColour.rgb);

	if (luminance<0.4f)
	{
		colour=vec4(0.0f,0.0f,0.0f,1.0f);
	}
	else
	{
		colour=vec4(1.0f,1.0f,1.0f,1.0f);
	}
}