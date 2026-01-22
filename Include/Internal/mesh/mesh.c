#include "./mesh.h"

#include "../../External/glad/glad.h"

#include <string.h>

void SetupMesh(Mesh *mesh);

// Initializes mesh
void NewMesh(Mesh *mesh, List vertices, List indices, List textures) {
  // Initializing mesh variables
  mesh->vertices = vertices;
  mesh->indices = indices;
  mesh->textures = textures;

  // Mesh setup
  SetupMesh(mesh);
};

// Setups meshes with information
void SetupMesh(Mesh *mesh) {
  unsigned int *VAO = &mesh->VAO;
  unsigned int *VBO = &mesh->VBO;
  unsigned int *EBO = &mesh->EBO;

  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  glGenBuffers(1, EBO);

  glBindVertexArray(*VAO);

  // Creating pointers arrays from list
  Vertex **vertexPointers = (Vertex **)ListToArray(&mesh->vertices);
  unsigned int **indexPointers = (unsigned int **)ListToArray(&mesh->indices);

  size_t numVertices = mesh->vertices.size;
  size_t numIndices = mesh->indices.size;

  // Setting arrays from pointer arrays
  Vertex vertices[numVertices];
  unsigned int indices[numIndices];

  // Vertices
  for (int i = 0; i < numVertices; i++)
    vertices[i] = *vertexPointers[i];

  // Indices
  for (int i = 0; i < numIndices; i++)
    indices[i] = *indexPointers[i];

  // Binding buffers
  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), &vertices[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  // Positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

  // Normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Normal));

  // Texture coordinates
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, TexCoord));

  glBindVertexArray(0);

  // Freeing arrays
  free(vertexPointers);
  free(indexPointers);
};

// Draws mesh using textures
void DrawMesh(Mesh *mesh, Shader *shader) {
  unsigned int diffuseNumber = 0;
  unsigned int specularNumber = 0;

  // Creating pointers array from list
  size_t numTextures = mesh->textures.size;
  Texture **texturePointers = (Texture **)ListToArray(&mesh->textures);

  // Getting texture array
  Texture textures[numTextures];
  for (int i = 0; i < numTextures; i++)
    textures[i] = *texturePointers[i];

  // Texture looping
  for (unsigned int i = 0; i < numTextures; i++) {
    glActiveTexture(GL_TEXTURE0 + i);

    // Retrieve texture number
    char *name = textures[i].type;
    int number;

    if (strcmp(name, "textureDiffuse") == 0)
      number = diffuseNumber++;
    else if (strcmp(name, "textureSpecular") == 0)
      number = specularNumber++;

    char buffer[128];
    int size = snprintf(buffer, sizeof(buffer), "material.%s%d", name, number);
    if (size < 0 || size >= sizeof(buffer)) {
      printf("Invalid buffer size while setting texture: %d", size);
      return;
    };

    SetInt(shader, buffer, i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  };

  glActiveTexture(GL_TEXTURE0);

  // Draw mesh
  glBindVertexArray(mesh->VAO);
  glDrawElements(GL_TRIANGLES, mesh->indices.size, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // Free texture array
  free(texturePointers);
};
