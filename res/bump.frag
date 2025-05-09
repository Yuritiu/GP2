#version 400 core

in VS_OUT {
    vec2  tC;
    vec3  FragPos;
    mat3  TBN;
} fs_in;

out vec4 FragColour;

// texture samplers
uniform sampler2D diffuse;   // unit 0
uniform sampler2D normalT;   // unit 1
uniform vec2     tiling;     // how many times to repeat

// lighting uniforms 
uniform vec3 lightPos;       // world-space light position
uniform vec3 lightColor;     // e.g. (1,1,1)
uniform vec3 ambientColor;   // e.g. (0.1,0.1,0.1)
uniform vec3 viewPos;        // camera position

uniform float lightConstant;   // e.g. 1.0
uniform float lightLinear;     // e.g. 0.014
uniform float lightQuadratic;  // e.g. 0.0007

void main() {
    vec2 uv    = fs_in.tC * tiling;
    vec3  color = texture(diffuse, uv).rgb;
    vec3  nmap  = texture(normalT, uv).rgb * 2.0 - 1.0; 
    vec3  norm  = normalize(fs_in.TBN * nmap);

    float dist = length(lightPos - fs_in.FragPos);
    float attenuation = 1.0 
                      / (lightConstant 
                         + lightLinear * dist 
                         + lightQuadratic * dist * dist);
    vec3 ambientTerm = ambientColor * color * attenuation;

    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diffStrength = max(dot(norm, lightDir), 0.0);
    vec3 diffuseTerm = lightColor * diffStrength * attenuation * color;

    vec3 viewDir   = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir= reflect(-lightDir, norm);
    float specStrength = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specularTerm  = lightColor * specStrength * attenuation;

    vec3 result = ambientTerm + diffuseTerm + specularTerm;
    FragColour = vec4(result, 1.0);
}
