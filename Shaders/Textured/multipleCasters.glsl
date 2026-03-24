#version 330 core
struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shine;
};

struct DirectionalLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

struct SpotLight {
  vec3 position;
  vec3 direction;

  float cutOff;
  float outerCutOff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

#define POINT_LIGHTS_AMOUNT 4

uniform Material material;
uniform PointLight pointLights[POINT_LIGHTS_AMOUNT];
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;

uniform vec3 viewPosition;

in vec3 FragPosition;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

vec3 CalculateDirLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDirection);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDirection);

void main() {
  // Calculating properties
  vec3 viewDirection = normalize(viewPosition - FragPosition);
  vec3 normal = normalize(Normal);

  // Directional light
  vec3 result = CalculateDirLight(directionalLight, normal, viewDirection);

  // Point light
  for (int i = 0; i < POINT_LIGHTS_AMOUNT; i++) {
    result += CalculatePointLight(pointLights[i], normal, viewDirection);
  }

  // Spot light
  result += CalculateSpotLight(spotLight, normal, viewDirection);

  FragColor = vec4(result, 1.0);
};

vec3 CalculateDirLight(DirectionalLight light, vec3 normal, vec3 viewDirection) {
  // Calculating diffuse color
  vec3 diffuseColor = vec3(texture(material.diffuse, TexCoord));

  // Ambient lighting
  vec3 ambient = light.ambient * diffuseColor;

  // Diffuse lighting
  vec3 lightDirection = normalize(-light.direction);
  vec3 diffuse = light.diffuse * max(dot(lightDirection, normal), 0.0) * diffuseColor;

  // Specular lighting
  vec3 reflectDirection = reflect(-lightDirection, normal);

  float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shine);
  vec3 specularColor = vec3(texture(material.specular, TexCoord));
  vec3 specular = light.specular * specularValue * specularColor;

  return (ambient + diffuse + specular);
};

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDirection) {
  // Calculating diffuse color
  vec3 diffuseColor = vec3(texture(material.diffuse, TexCoord));

  // Ambient lighting
  vec3 ambient = light.ambient * diffuseColor;

  // Diffuse lighting
  vec3 distanceVector = light.position - FragPosition;
  vec3 lightDirection = normalize(distanceVector);
  vec3 diffuse = light.diffuse * max(dot(lightDirection, normal), 0.0) * diffuseColor;

  // Specular lighting
  vec3 reflectDirection = reflect(-lightDirection, normal);

  float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shine);
  vec3 specularColor = vec3(texture(material.specular, TexCoord));
  vec3 specular = light.specular * specularValue * specularColor;

  // Attenuation of light
  float dist = length(distanceVector);
  float attenuation = 1. / (light.constant + light.linear * dist + light.quadratic * pow(dist, 2));

  // Atennuating all types of light
  return (ambient + diffuse + specular) * attenuation;
};

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDirection) {
  // Calculating diffuse color
  vec3 diffuseColor = vec3(texture(material.diffuse, TexCoord));

  // Calculating light
  vec3 distanceVector = light.position - FragPosition;
  vec3 lightDirection = normalize(distanceVector);
  float theta = dot(-lightDirection, normalize(light.direction));

  // Ambient
  vec3 ambient = light.ambient * diffuseColor;

  // If object is not hit by spotlight
  if (theta <= light.outerCutOff) return ambient;

  // Calculating intensity
  float eps = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / eps, 0.0, 1.0);

  // Diffuse
  vec3 diffuse = light.diffuse * max(dot(lightDirection, normal), 0.0) * diffuseColor;

  // Specular lighting
  vec3 reflectDirection = reflect(-lightDirection, normal);

  float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shine);
  vec3 specularColor = vec3(texture(material.specular, TexCoord));
  vec3 specular = light.specular * specularValue * specularColor;

  // Attenuation of light
  float dist = length(distanceVector);
  float attenuation = 1. / (light.constant + light.linear * dist + light.quadratic * pow(dist, 2));

  return ambient + (diffuse + specular) * intensity * attenuation;
};
