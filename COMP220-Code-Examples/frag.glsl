#version 330 core

out vec4 color;

in vec4 vertexColoursOut;

void main(){
  color = vertexColoursOut;
}