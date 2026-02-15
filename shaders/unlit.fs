#version 410 core

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D textureSampler;
uniform vec3 tint = vec3(1.0);

void main()
{
    FragColor = texture(textureSampler, TexCoords) * vec4(tint, 1.0);
}