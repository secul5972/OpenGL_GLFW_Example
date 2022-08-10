#version 330 core

in vec3 lightingColor;

uniform vec3 objectColor;

out vec4 FragColor;

void main()
{
	FragColor = vec4(lightingColor * objectColor, 1.0);
}