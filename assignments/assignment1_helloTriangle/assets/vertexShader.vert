#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform float _Time;

void main()
{
    vec3 pos = aPos;
    pos.y += sin(_Time * 5.0 + pos.x) * 0.5;

    gl_Position = vec4(pos, 1.0);
    vertexColor = aColor;
}
