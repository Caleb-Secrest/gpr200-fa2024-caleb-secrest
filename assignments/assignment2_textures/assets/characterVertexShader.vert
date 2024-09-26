#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform vec2 position;
uniform vec2 scale;

void main()
{
    vec3 scaledPosition = aPos * vec3(scale, 1.0) + vec3(position, 0.0);
    gl_Position = vec4(scaledPosition.xy, 0.0, 1.0);
    TexCoord = aTexCoord;
}