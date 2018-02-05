#version 330 core

out vec4 outColor;
uniform vec4 color;

void main()
{
	outColor = color;
	//outColor = vec4(1.0, 0.0, 0.0, 1.0);
} 