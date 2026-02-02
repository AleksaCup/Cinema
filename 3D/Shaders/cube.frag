#version 330 core

struct Light {
    vec3 pos;
    vec3 kA;
    vec3 kD;
    vec3 kS;
};

struct Material {
    vec3 kA;
    vec3 kD;
    vec3 kS;
    float shine;
};

in vec3 chNor;
in vec3 chFragPos;

out vec4 outCol;

uniform Light uRoomLight;
uniform Light uScreenLight;

uniform bool uRoomLightOn;
uniform bool uScreenLightOn;

uniform Material uMaterial;
uniform vec3 uViewPos;

vec3 applyLight(Light light, vec3 normal, vec3 viewDir)
{
    // ambient
    vec3 resA = light.kA * uMaterial.kA;

    // diffuse
    vec3 lightDir = normalize(light.pos - chFragPos);
    float nD = max(dot(normal, lightDir), 0.0);
    vec3 resD = light.kD * (nD * uMaterial.kD);

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float s = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shine);
    vec3 resS = light.kS * (s * uMaterial.kS);

    return resA + resD + resS;
}

void main()
{
    vec3 normal = normalize(chNor);
    vec3 viewDir = normalize(uViewPos - chFragPos);

    vec3 color = vec3(0.0);

    if (uRoomLightOn)
        color += applyLight(uRoomLight, normal, viewDir);

    if (uScreenLightOn)
        color += applyLight(uScreenLight, normal, viewDir);

    outCol = vec4(color, 1.0);
}
