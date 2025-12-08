#version 330 core

layout (location = 0) in vec2 aPos;
uniform vec2 uPos;
uniform float uSize;
void main()
{
    vec2 scaled = aPos * uSize;
    gl_Position = vec4(scaled + uPos, 0.0, 1.0);
}
