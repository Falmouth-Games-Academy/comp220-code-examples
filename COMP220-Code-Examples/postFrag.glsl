#version 330 core
out vec4 color;
in vec2 textureCoords;

uniform sampler2D texture0;

void main()
{
	//Read the texture and do some processing!
	color = texture(texture0, textureCoords);

}