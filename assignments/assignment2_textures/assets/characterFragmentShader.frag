#version 330 core

out vec4 fragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D characterTexture;

void main() {
    if (fragColor.a < 0.1) discard;
    fragColor = texture(characterTexture, TexCoord);
}
