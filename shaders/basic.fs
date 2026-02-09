#version 330 core

in vec4 color;
in vec2 texCoord;

out vec4 FragColor;

//uniform vec4 color;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float opacity;

void main()
{
  //texture() function takes a sampler then our texture vectors
  FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), opacity); 
}