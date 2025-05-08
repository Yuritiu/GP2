#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in vec3 vertexNormal;

uniform mat4 transform;

out vec3 normal;
out vec3 viewDirection;

uniform mat4 modelMatrix;
uniform vec3 camPos;

void main()
{
    vec4 worldPosition = transform * vec4(vertexPosition, 1.0);
    vec4 viewPosition = vec4(worldPosition.xyz - camPos, 1.0);
    gl_Position = transform * vec4(vertexPosition, 1.0);

    normal = mat3(transpose(inverse(modelMatrix))) * vertexNormal;
    viewDirection = -vec3(viewPosition);
}