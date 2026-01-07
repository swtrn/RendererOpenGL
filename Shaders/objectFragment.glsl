#version 330 core
out vec4 FragColor;

in vec3 FragPosition;
in vec2 TexCoord;
in vec3 Normal;

// Not in use
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPosition;
uniform vec3 viewPosition;

const float specularStrength = 0.5;
const float shineStrength = 64;

void main() {
  // Ambient lighting
  float ambient = 0.1;

  // Diffuse lighting
  vec3 lightDirection = normalize(lightPosition - FragPosition);
  vec3 normal = normalize(Normal);

  float diffuse = max(dot(lightDirection, normal), 0.0);

  // Specular lighting
  vec3 viewDirection = normalize(viewPosition - FragPosition);
  vec3 reflectDirection = reflect(-lightDirection, normal);

  float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), shineStrength);
  float specular = specularValue * specularStrength;

  FragColor = vec4((ambient + diffuse + specular) * lightColor * objectColor, 1.0);
} 
