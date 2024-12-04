#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

void main() {
    vec3 color = vec3(0.0, 0.5, 0.8); // Ocean blue
    FragColor = vec4(color, 1.0);
}
