#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Norm;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform bool blinn;
uniform float ambient;
uniform float specularStrength;
uniform float diffuseStrength;
uniform float shine;

void main()
{
    vec3 amb = ambient * lightColor;

    vec3 norm = normalize(Norm);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = 0.0;

    if (blinn)
    {
        vec3 halfDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(norm, halfDir), 0.0), shine);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), shine);
    }

    vec3 specular = specularStrength * spec * lightColor;

    vec3 lighting = amb + diffuse + specular;
    vec4 texColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

    FragColor = vec4(texColor.rgb * lighting, texColor.a);
}