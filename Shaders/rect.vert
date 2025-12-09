#version 330 core

layout(location = 0) in vec2 aPos;

uniform vec4 uRect; // x, y, w, h

void main()
{
    vec2 scaled = aPos * uRect.zw + uRect.xy;
    gl_Position = vec4(scaled, 0.0, 1.0);
}
