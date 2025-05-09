#version 400 core
out vec4 FragColor;

uniform float shadowAlpha;

void main() 
{
    FragColor = vec4(0.0, 0.0, 0.0, shadowAlpha);
}
