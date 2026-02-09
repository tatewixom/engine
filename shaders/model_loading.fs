#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_base_1;
uniform sampler2D texture_base_2;

uniform sampler2D texture_norm_1;
uniform sampler2D texture_norm_2;

void main()
{    
  //FragColor = vec4(TexCoords, 0.f, 1.f);
  FragColor = texture(texture_base_1, TexCoords);
  //FragColor = texture(texture_base_1, TexCoords) * vec4(1, 0, 1, 1);
}