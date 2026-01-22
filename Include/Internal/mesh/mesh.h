#ifndef MESH_H
#define MESH_H

#include "../../External/cglm/include/cglm/cglm.h"
#include "../list/list.h"
#include "../shader/shader.h"

struct Vertex {
  vec3 Position;
  vec3 Normal;
  vec2 TexCoord;
} typedef Vertex;

struct Texture {
  unsigned int id;
  char *type;
  char *path;
} typedef Texture;

struct Mesh {
  unsigned int VAO, VBO, EBO;

  List indices;
  List vertices;
  List textures;
} typedef Mesh;

void NewMesh(Mesh *mesh, List vertices, List indices, List textures);
void DrawMesh(Mesh *mesh, Shader *shader);

#endif
