#ifndef TEXTURE_H
#define TEXTURE_H

void LoadTexture(int target, unsigned int *texture, char *texturePath);
void UseTexture(int texture, int index, int target);

#endif
