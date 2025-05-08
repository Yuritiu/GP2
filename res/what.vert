// Vertex shader
#version 440

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 normal;

out vec3 v_normal;
out vec3 v_worldPos;

uniform mat4 transform;

void main()
{
    v_normal = mat3(transpose(inverse(transform))) * normal;
    
    vec4 wp = transform * vec4(position, 1.0);
    v_worldPos = wp.xyz;
    gl_Position = transform * vec4(position, 1.0);
}