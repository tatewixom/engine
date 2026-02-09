#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in mat4 aModel; // total locations are 4, 5, 6, 7


uniform mat4 mvp;

out vec3 FragPos; //position of fragment
out vec3 Normal;
out vec2 TexCoords;

void main()
{
  FragPos = vec3(aModel * vec4(aPos, 1.0));

  TexCoords = aTexCoords;

  Normal = mat3(transpose(inverse(aModel))) * aNormal;

  gl_Position = mvp * aModel * vec4(aPos, 1.0);
}