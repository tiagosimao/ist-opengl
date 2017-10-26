#version 330 core

in vec4 in_Position;
in vec4 in_Color;
out vec4 ex_Color;

uniform mat4 projectionMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

void main(void)
{
	gl_Position = projectionMat * viewMat * modelMat * in_Position;
	ex_Color = in_Color;
}
