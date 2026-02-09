#version 330 core

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct LightCaster
{
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct LightPoint
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
in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

uniform vec3 objectColor;

uniform vec3 viewPos;

uniform Material material;
uniform LightCaster lightCaster;
uniform LightPoint lightPoint;

void main()
{
  //ambient
  vec3 ambient = lightPoint.ambient * vec3(texture(material.diffuse, TexCoords)); //ambient light

  //diffuse
  vec3 norm = normalize(normal); //normalizing normal vector to be between 1 - 0
  vec3 lightDir = normalize(lightPoint.position - fragPos); //finding direction vector of light source
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diff * lightPoint.diffuse;

  //specular & shininess
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float specularStrength = 0.5;
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 32 is the shinines of the highlight
  vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * lightPoint.specular;

  //attenuation
  float distance = length(lightPoint.position - fragPos);
  float attenuation = 1.0 / (lightPoint.constant + lightPoint.linear * distance + lightPoint.quadratic * (distance * distance));   
  ambient  *= attenuation; 
  diffuse  *= attenuation;
  specular *= attenuation; 

  //final result
  vec3 result = (ambient + diffuse + specular) * objectColor;
  FragColor = vec4(result, 1.0); //final result
}

/* 

void main()
{
  //ambient
  vec3 ambient = lightCaster.ambient * vec3(texture(material.diffuse, TexCoords)); //ambient light

  //diffuse
  vec3 norm = normalize(normal); //normalizing normal vector to be between 1 - 0
  vec3 lightDir = normalize(-lightCaster.direction); //finding direction vector of light source
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diff * lightCaster.diffuse;

  //specular & shininess
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float specularStrength = 0.5;
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 32 is the shinines of the highlight
  vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * lightCaster.specular;

  //final result
  vec3 result = (ambient + diffuse + specular) * objectColor;
  FragColor = vec4(result, 1.0); //final result
}

*/