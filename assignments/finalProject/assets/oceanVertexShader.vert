#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main() {
    vec3 position = aPos;
    position.y = sin(position.x * 0.1 + time) * 0.5 + cos(position.z * 0.1 + time) * 0.5;
    gl_Position = projection * view * model * vec4(position, 1.0);
    TexCoords = aTexCoords;
}
