#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_base_1;
uniform sampler2D texture_base_2;
uniform sampler2D texture_base_3;
uniform sampler2D texture_base_4;
uniform sampler2D texture_base_5;
uniform sampler2D texture_base_6;
uniform sampler2D texture_base_7;
uniform sampler2D texture_base_8;
uniform sampler2D texture_base_9;
uniform sampler2D texture_base_10;
uniform sampler2D texture_base_11;
uniform sampler2D texture_base_12;
uniform sampler2D texture_base_13;
uniform sampler2D texture_base_14;
uniform sampler2D texture_base_15;
uniform sampler2D texture_base_16;

uniform sampler2D texture_norm_1;
uniform sampler2D texture_norm_2;
uniform sampler2D texture_norm_3;
uniform sampler2D texture_norm_4;
uniform sampler2D texture_norm_5;
uniform sampler2D texture_norm_6;
uniform sampler2D texture_norm_7;
uniform sampler2D texture_norm_8;
uniform sampler2D texture_norm_9;
uniform sampler2D texture_norm_10;
uniform sampler2D texture_norm_11;
uniform sampler2D texture_norm_12;
uniform sampler2D texture_norm_13;
uniform sampler2D texture_norm_14;
uniform sampler2D texture_norm_15;
uniform sampler2D texture_norm_16;

void main()
{    
  //FragColor = vec4(TexCoords, 0.f, 1.f);
  FragColor = texture(texture_base_1, TexCoords);
  //FragColor = texture(texture_base_1, TexCoords) * vec4(1, 0, 1, 1);
}