#version 330

in vec3 normal;
in vec3 viewDirection;

out vec4 fragmentColor;

uniform vec3 rimColor;
uniform float rimPower;

uniform sampler2D diffuse;

void main()
{
    float rim = pow(1.0 - max(dot(normalize(normal), normalize(viewDirection)), 0.0), rimPower);
    fragmentColor = vec4(rimColor, 1.0) * rim;
}