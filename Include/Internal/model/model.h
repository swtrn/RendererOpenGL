#ifndef MODEL_H
#define MODEL_H

#include "../mesh/mesh.h"

struct Model {
  List meshes;
  char *directory;
} typedef Model;

void LoadModel(Model *model, const char *path);
void DrawModel(Model *model, Shader *shader);

#endif
