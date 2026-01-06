#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
// in vec3 ourColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
  FragColor = vec4(lightColor * objectColor, 1.0);
  // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 1);
  // FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
} 
