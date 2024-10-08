#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float tiling;

void main()
{
    vec2 tiledUV = TexCoord * tiling;
    vec4 texColor1 = texture(texture1, tiledUV);
    vec4 texColor2 = texture(texture2, tiledUV);

    FragColor = mix(texColor1, texColor2, 0.5);
}
