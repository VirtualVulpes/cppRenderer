#version 410 core

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
};
uniform Material material;

uniform vec3 tint = vec3(1.0);

void main()
{
    FragColor = texture(material.diffuse, TexCoords) * vec4(tint, 1.0);
}