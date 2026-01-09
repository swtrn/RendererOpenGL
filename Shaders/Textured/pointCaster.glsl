#version 330 core
struct Material { 
  sampler2D diffuse;
  sampler2D specular;
  float shine; 
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

uniform Material material;
uniform Light light;

uniform vec3 viewPosition;

out vec4 FragColor;

in vec3 FragPosition;
in vec3 Normal;

in vec2 TexCoord;

void main() {
  vec3 diffuseColor = vec3(texture(material.diffuse, TexCoord));

  // Ambient lighting
  vec3 ambient = light.ambient * diffuseColor;

  // Diffuse lighting
  vec3 distanceVector = light.position - FragPosition;
  vec3 lightDirection = normalize(distanceVector);
  float distance = length(distanceVector);

  // Attenuation of light
  float attenuation = 1. / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2));

  vec3 normal = normalize(Normal);

  vec3 diffuse = light.diffuse * max(dot(lightDirection, normal), 0.0) * diffuseColor;

  // Specular lighting
  vec3 viewDirection = normalize(viewPosition - FragPosition);
  vec3 reflectDirection = reflect(-lightDirection, normal);

  float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shine);
  vec3 specularColor = vec3(texture(material.specular, TexCoord));
  vec3 specular = light.specular * specularValue * specularColor;

  // Atennuating all types of light
  vec3 result = (ambient + diffuse + specular) * attenuation;

  FragColor = vec4(result , 1.0);
}
