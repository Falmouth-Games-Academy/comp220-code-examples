#version 330 core
out vec4 color;
in vec2 textureCoords;

uniform sampler2D texture0;

void main()
{
	//Read the texture and do some processing!
	vec2 lookup=vec2(1.0f,1.0f);

	vec4 textureColour = texture(texture0, textureCoords);
	
	float luminance = (textureColour.r+textureColour.g+textureColour.b)/3;

	if (luminance <0.2f)
	{
		color=vec4(0.0f,0.0f,0.0f,1.0f);
	}
	else
	{
		color=vec4(1.0f,1.0f,1.0f,1.0f);
	}

}