#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 vertexColor;
out vec2 TexCoord;

uniform float _Time;
uniform vec2 position;
uniform vec2 scale;

void main()
{
    vec3 pos = aPos;

    pos.y += sin(_Time * 2.0) * 0.75;

    float stretch = sin(_Time * 3.0) * 0.5;
    mat4 rotation = mat4(
        cos(stretch), 0.0, sin(stretch), 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sin(stretch), 0.0, cos(stretch), 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    pos = vec3(rotation * vec4(pos, 1.0));

    pos *= vec3(scale, 1.0);
    pos += vec3(position, 0.0);

    gl_Position = vec4(pos, 1.0);
    vertexColor = aColor;
    TexCoord = aTexCoord;
}
