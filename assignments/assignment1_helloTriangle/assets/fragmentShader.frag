#version 330 core

in vec3 vertexColor;
out vec4 FragColor;

uniform float _Time;

void main()
{
    vec3 color = vertexColor;
    color.r = abs(sin(_Time * 2.0 + vertexColor.r * 10.0));
    color.g = abs(sin(_Time * 2.0 + vertexColor.g * 10.0));
    color.b = abs(sin(_Time * 2.0 + vertexColor.b * 10.0));

    FragColor = vec4(color, 1.0);
}
