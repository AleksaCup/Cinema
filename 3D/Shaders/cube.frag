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
in vec2 chTex;

out vec4 outCol;

uniform Light uRoomLight;
uniform Light uScreenLight;

uniform bool uRoomLightOn;
uniform bool uScreenLightOn;

uniform Material uMaterial;
uniform vec3 uViewPos;

uniform sampler2D uTexture;
uniform vec3 uTint;

vec3 applyLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 resA = light.kA * uMaterial.kA;

    vec3 lightDir = normalize(light.pos - chFragPos);
    float nD = max(dot(normal, lightDir), 0.0);
    vec3 resD = light.kD * (nD * uMaterial.kD);

    vec3 reflectDir = reflect(-lightDir, normal);
    float s = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shine);
    vec3 resS = light.kS * (s * uMaterial.kS);

    return resA + resD + resS;
}

void main()
{
    vec3 normal = normalize(chNor);
    vec3 viewDir = normalize(uViewPos - chFragPos);

    vec3 lighting = uMaterial.kA * 0.3;

    if (uRoomLightOn)
        lighting += applyLight(uRoomLight, normal, viewDir);

    if (uScreenLightOn)
        lighting += applyLight(uScreenLight, normal, viewDir);

    vec4 texColor = texture(uTexture, chTex);   // ⬅ NOVO

    vec3 finalColor = texColor.rgb * lighting * uTint;

    outCol = vec4(finalColor, texColor.a);
}
