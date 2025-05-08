#version 400 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec2  tC;
    vec3  FragPos;
    mat3  TBN;
} vs_out;

void main() {
    vs_out.tC = aTexCoord;
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));

    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(mat3(model) * aNormal);
    vs_out.TBN = mat3(T, B, N);

    gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}
