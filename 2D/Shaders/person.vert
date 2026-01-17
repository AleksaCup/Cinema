#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

uniform vec2 uPos;
uniform float uSize;
uniform int uFlipY;

out vec2 vUV;

void main()
{
    if (uFlipY == 0)
        vUV = vec2(aUV.x, 1.0 - aUV.y);
    else
        vUV = aUV;

    vec2 scaled = aPos * uSize;
    gl_Position = vec4(scaled + uPos, 0.0, 1.0);
}
