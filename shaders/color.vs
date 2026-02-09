#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormal;

uniform mat4 mvp;
uniform mat4 model;

out vec3 fragPos; //position of fragment
out vec3 normal;
out vec2 TexCoords;

void main()
{   
    fragPos = vec3(model * vec4(aPos, 1.0));

    TexCoords = aTexCoords;

    normal = mat3(transpose(inverse(model))) * aNormal; //transposing as to gaurd against scaling errors

    gl_Position = mvp * vec4(aPos, 1.0);
} 