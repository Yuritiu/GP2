#version 400 core

in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D reflectTex;

void main() 
{
    FragColor = texture(reflectTex, TexCoords);
}