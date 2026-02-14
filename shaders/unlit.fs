#version 410 core

out vec4 FragColor;

in vec3 ourColor;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D textureSampler;

void main()
{
    FragColor = texture(textureSampler, TexCoord);
    //FragColor = vec4(Normal, 1.0);
}