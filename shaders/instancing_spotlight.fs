#version 330 core

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Spotlight
{
  vec3 position;
  vec3 direction;
  float cutoff;
  float outerCutoff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform Material material;
uniform Spotlight spotlight;

void main()
{
  //ambient
  vec3 ambient = spotlight.ambient * vec3(texture(material.diffuse, TexCoords)); //ambient light

  //diffuse
  vec3 norm = normalize(Normal); //normalizing normal vector to be between 1 - 0
  vec3 lightDir = normalize(spotlight.position - FragPos); //finding direction vector of light source
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diff * spotlight.diffuse;

  //specular & shininess
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float specularStrength = 0.5;
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 32 is the shinines of the highlight
  vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * spotlight.specular;

  //spotlight 
  float theta = dot(lightDir, normalize(-spotlight.direction));
  float epsilon = spotlight.cutoff - spotlight.outerCutoff;
  float intensity = clamp((theta - spotlight.outerCutoff) / epsilon, 0.0, 1.0);  
  diffuse *= intensity;
  specular *= intensity;

  //attenuation
  float distance = length(spotlight.position - FragPos);
  float attenuation = 1.0 / (spotlight.constant + spotlight.linear * distance + spotlight.quadratic * (distance * distance));   
  ambient  *= attenuation; 
  diffuse  *= attenuation;
  specular *= attenuation; 

  //final result
  vec3 result = (ambient + diffuse + specular) * objectColor;
  FragColor = vec4(result, 1.0); //final result
}