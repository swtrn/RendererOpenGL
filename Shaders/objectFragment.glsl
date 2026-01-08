#version 330 core
struct Material { 
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shine; 
};

uniform Material material;

uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 lightColor;

// Not in use
uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

in vec3 FragPosition;
// in vec2 TexCoord;
in vec3 Normal;

void main() {
 // Ambient lighting
  vec3 ambient = lightColor * material.ambient;

  // Diffuse lighting
  vec3 lightDirection = normalize(lightPosition - FragPosition);
  vec3 normal = normalize(Normal);

  vec3 diffuse = lightColor * (max(dot(lightDirection, normal), 0.0) * material.diffuse);

  // Specular lighting
  vec3 viewDirection = normalize(viewPosition - FragPosition);
  vec3 reflectDirection = reflect(-lightDirection, normal);

  float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shine);
  vec3 specular = lightColor * (specularValue * material.specular);

  FragColor = vec4((ambient + diffuse + specular), 1.0);
} 
