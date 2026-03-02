#version 330 core

struct Light
{
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 u_viewPos;
uniform Light u_light;

uniform sampler2D texture_base_1;
uniform sampler2D texture_base_2;
uniform sampler2D texture_base_3;

uniform sampler2D texture_norm_1;
uniform sampler2D texture_norm_2;
uniform sampler2D texture_norm_3;

uniform sampler2D texture_meta_1;
uniform sampler2D texture_meta_2;
uniform sampler2D texture_meta_3;

void main()
{
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(u_light.position - FragPos);
  vec3 viewDir = normalize(u_viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float distance = length(u_light.position - FragPos);
  float attenuation = 1.0 / (u_light.constant + u_light.linear * distance + u_light.quadratic * (distance * distance));

  //ambient
  vec3 ambient = u_light.ambient * vec3(texture(texture_base_1, TexCoords));

  //diffuse
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = u_light.diffuse * diff * vec3(texture(texture_base_1, TexCoords));

  //result
  ambient *= attenuation;
  diffuse *= attenuation;

  FragColor = vec4(ambient + diffuse, 1.0);
}