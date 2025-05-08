#version 400 core

in VS_OUT {
    vec2  tC;
    vec3  FragPos;
    mat3  TBN;
} fs_in;

uniform sampler2D diffuse;   // bind your color map here
uniform sampler2D normalT;   // bind your normal map here
uniform vec2 tiling;   // how many times to repeat the texture

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 viewPos;

out vec4 FragColour;

void main() {
    vec2 uv = fs_in.tC * tiling;

    vec3 nmap   = texture(normalT, uv).rgb;
    vec3 normal = normalize(nmap * 2.0 - 1.0);
    normal       = normalize(fs_in.TBN * normal);

    vec3 color = texture(diffuse, uv).rgb;

    vec3 ambient = ambientColor * color;

    vec3 L = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(normal, L), 0.0);
    vec3 diffuseCol = diff * lightColor * color;

    vec3 V = normalize(viewPos - fs_in.FragPos);
    vec3 R = reflect(-L, normal);
    float spec = pow(max(dot(V, R), 0.0), 32.0);
    vec3 specular = spec * lightColor;

    FragColour = vec4(ambient + diffuseCol + specular, 1.0);
}
