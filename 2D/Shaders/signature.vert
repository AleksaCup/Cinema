#version 330 core

layout (location = 0) in vec2 aPos;     // X, Y pozicija u NDC
layout (location = 1) in vec2 aTex;     // UV koordinate teksture

out vec2 vTex;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    vTex = aTex;
}
