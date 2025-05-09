#version 400 core

in vec3 WorldPos; 
in vec3 Normal;   
in vec2 TexCoords;

out vec4 FragColor;

uniform samplerCube skybox;
uniform sampler2D reflectTex;

uniform bool  usePlanar;
uniform vec3  cameraPos;

void main() 
{
    if (usePlanar) 
    {
        vec2 uv = vec2(TexCoords.x, TexCoords.y);
        FragColor = texture(reflectTex, uv);
    }
    else 
    {
        vec3 I = normalize(WorldPos - cameraPos);
        vec3 R = reflect(I, normalize(Normal));
        vec3 envColor = texture(skybox, R).rgb;
        FragColor = vec4(envColor, 1.0);
    }
}
