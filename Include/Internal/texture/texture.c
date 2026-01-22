#include "./texture.h"

#include "../../External/glad/glad.h"
#include "../../External/stb_image/stb_image.h"

#include <stdbool.h>

// Loads a texture
void LoadTexture(int target, unsigned int *texture, char *texturePath) {
  // Creating and binding textures
  glGenTextures(1, texture);
  glBindTexture(target, *texture);

  // Loading image
  int width, height, numberChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(texturePath, &width, &height, &numberChannels,
                                  0); // Triangle vertices

  // Checking data load
  if (!data) {
    printf("Failed to load texture.");
    stbi_image_free(data);
    return;
  }

  GLenum format;
  switch (numberChannels) {
  case 1:
    format = GL_RED;
  case 3:
    format = GL_RGB;
  case 4:
    format = GL_RGBA;
  }

  // Generating texture
  glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE,
               data);
  glGenerateMipmap(target);

  glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Freeing allocated image data
  stbi_image_free(data);
}

// Binds Textures
void UseTexture(int texture, int index, int target) {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(target, texture);
}
