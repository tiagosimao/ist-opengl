#version 330 core

in vec4 ex_Color;
out vec4 out_Color;

uniform vec4 uColor;

void main(void)
{
	out_Color = uColor;
}
