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
  if (data) {
    // Generating texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("Failed to load texture.");
  }

  // Freeing allocated image data
  stbi_image_free(data);
}

// Binds Textures
void UseTexture(int texture, int index, int target) {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(target, texture);
}
