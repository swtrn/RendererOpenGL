#version 330 core
struct Material { 
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shine; 
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Material material;
uniform Light light;

uniform vec3 viewPosition;

// Not in use
uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

in vec3 FragPosition;
// in vec2 TexCoord;
in vec3 Normal;

void main() {
 // Ambient lighting
  vec3 ambient = light.ambient * material.ambient;

  // Diffuse lighting
  vec3 lightDirection = normalize(light.position - FragPosition);
  vec3 normal = normalize(Normal);

  vec3 diffuse = light.diffuse * (max(dot(lightDirection, normal), 0.0) * material.diffuse);

  // Specular lighting
  vec3 viewDirection = normalize(viewPosition - FragPosition);
  vec3 reflectDirection = reflect(-lightDirection, normal);

  float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shine);
  vec3 specular = light.specular * (specularValue * material.specular);

  FragColor = vec4((ambient + diffuse + specular), 1.0);
} 
