#version 400

in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skybox;

void main() 
{
    vec3 tc = TexCoords;

    FragColor = texture(skybox, tc);
}