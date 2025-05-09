#version 400 core

layout(location=0) in vec3 inPos;
layout(location=1) in vec2 inTex;
out vec2 TexCoords;
uniform mat4 model, view, projection;

void main() 
{
    TexCoords = inTex;
    gl_Position = projection * view * model * vec4(inPos,1);
}