#version 440

in vec3 v_normal;
in vec3 v_worldPos;
out vec4 outColor;

uniform vec3 cameraPosition;
uniform vec3 planetCenter;
uniform vec4 atmosphereColor;
uniform float atmosphereRadius;
uniform float planetRadius;

void main()
{
    vec3 ray = normalize(cameraPosition - v_worldPos);
    vec3 centerToRay = normalize(v_worldPos - planetCenter);
    float rayPlanetDistance = dot(centerToRay, ray);

    float planetIntersect = length(v_worldPos - planetCenter) - planetRadius;
    float atmosphereIntersect = length(v_worldPos - planetCenter) - atmosphereRadius;

    vec4 color = vec4(0.0, 0.9, 0.0, 1.0);

    if (planetIntersect < 0.0)
    {
        color = vec4(0.9, 0.0, 1.0, 0.5);
    }
    else if (atmosphereIntersect < 0.0)
    {
        float atmosphereDepth = 1.0 - (atmosphereIntersect / (atmosphereRadius - planetRadius));
        color = atmosphereColor * atmosphereDepth + vec4(1.0, 1.0, 1.0, 0.0) * (1.0 - atmosphereDepth);
    }

    outColor = color;
}


