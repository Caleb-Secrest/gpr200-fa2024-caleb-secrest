#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

out vec3 Norm;
out vec3 Pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    Norm = mat3(transpose(inverse(model))) * aNorm;
    Pos = vec3(model * vec4(aPos, 1.0));
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}