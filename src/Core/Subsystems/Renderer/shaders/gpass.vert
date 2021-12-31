#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 texCoords;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos   = model * vec4(aPos, 1.0);
    fragPos         = worldPos.xyz;
    texCoords       = aTexCoords;
    normal          = transpose(inverse(mat3(model))) * aNormal;

    gl_Position     = projection * view * worldPos;
}