#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform float rate;

void main()
{
   FragColor = mix(texture(Texture1, TexCoord),texture(Texture2, TexCoord), rate) ;
}