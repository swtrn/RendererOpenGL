#include "./model.h"

#include <GLFW/glfw3.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "../texture/texture.h"

typedef struct aiFace aiFace;
typedef struct aiMaterial aiMaterial;
typedef struct aiString aiString;

// Saving loaded textures
List loadedTextures; // TODO: Free loadedTextures on exit

void ProcessNode(Model *model, struct aiNode *node,
                 const struct aiScene *scene);

void ProcessMesh(Model *model, Mesh *mesh, struct aiMesh *AImesh,
                 const struct aiScene *scene);

void LoadMaterialTextures(Model *model, List *textures,
                          struct aiMaterial *material, enum aiTextureType type,
                          char *typeName);

// Draws model
void DrawModel(Model *model, Shader *shader) {
  for (Node *node = model->meshes.start; node; node = node->next)
    DrawMesh(node->data, shader);
};

// Imports model
void LoadModel(Model *model, const char *path) {
  const struct aiScene *scene =
      aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    printf("ERROR::ASSIMP::%s", aiGetErrorString());
    return;
  }

  // Finding directory
  int pathSize = strlen(path);
  for (int i = pathSize - 1; i > 0; i--) {
    if (path[i] == '/') {
      memcpy(model->directory, &path, i + 1);
      model->directory[i] = '\0';
      break;
    }
  };

  ProcessNode(model, scene->mRootNode, scene);
};

// Adds all meshes from a node to a model
void ProcessNode(Model *model, struct aiNode *node,
                 const struct aiScene *scene) {
  unsigned int numberMeshes = node->mNumMeshes;

  // Adding meshes
  for (unsigned int i = 0; i < numberMeshes; i++) {
    struct aiMesh *AImesh = scene->mMeshes[node->mMeshes[i]];

    Mesh mesh;
    ProcessMesh(model, &mesh, AImesh, scene);
    ListAppend(&model->meshes, &mesh, sizeof(Mesh));
  };

  // Processing children (recursion)
  unsigned int numberChildren = node->mNumMeshes;
  for (unsigned int i = 0; i < numberChildren; i++) {
    ProcessNode(model, node->mChildren[i], scene);
  };
};

// Process information inside each mesh
void ProcessMesh(Model *model, Mesh *mesh, struct aiMesh *AImesh,
                 const struct aiScene *scene) {
  // Creating lists
  List vertices, textures, indices;

  NewList(&vertices);
  NewList(&textures);
  NewList(&indices);

  unsigned int numVertices = AImesh->mNumVertices;
  unsigned int *index;
  for (unsigned int i = 0; i < numVertices; i++) {
    Vertex vertex;
    vec3 vector; // Vector holder

    // Getting positions
    vector[0] = AImesh->mVertices[i].x;
    vector[1] = AImesh->mVertices[i].y;
    vector[2] = AImesh->mVertices[i].z;
    memcpy(&vertex.Position, &vector, sizeof(vec3));

    // Getting normals
    vector[0] = AImesh->mNormals[i].x;
    vector[1] = AImesh->mNormals[i].y;
    vector[2] = AImesh->mNormals[i].z;
    memcpy(&vertex.Normal, &vector, sizeof(vec3));

    // Getting texture coordinates
    if (AImesh->mTextureCoords[0]) {
      vec2 vector_;
      vector_[0] = AImesh->mTextureCoords[0][i].x;
      vector_[1] = AImesh->mTextureCoords[0][i].y;
      memcpy(&vertex.TexCoord, &vector_, sizeof(vec2));
    } else
      memcpy(&vertex.TexCoord, &(vec2){0., 0.}, sizeof(vec2));

    ListAppend(&vertices, &vertex, sizeof(Vertex));
  };

  // Processing indices
  for (unsigned int i = 0; i < AImesh->mNumFaces; i++) {
    aiFace AIface = AImesh->mFaces[i];
    for (unsigned int j = 0; j < AIface.mNumIndices; j++)
      ListAppend(&indices, &AIface.mIndices[j], sizeof(unsigned int));
  };

  // Processing material
  if (AImesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[AImesh->mMaterialIndex];

    List diffuseMaps, specularMaps;
    NewList(&diffuseMaps);
    NewList(&specularMaps);

    // Getting all diffuse maps
    LoadMaterialTextures(model, &diffuseMaps, material, aiTextureType_DIFFUSE,
                         "textureDiffuse");
    // Getting all specular maps
    LoadMaterialTextures(model, &specularMaps, material, aiTextureType_SPECULAR,
                         "textureSpecular");

    // Appending diffuse maps to textures
    ListInsert(&textures, &diffuseMaps);
    ListInsert(&textures, &diffuseMaps);

    // Clearing lists
    ListClear(&diffuseMaps);
    ListClear(&specularMaps);
  };

  // Creating mesh with everything
  NewMesh(mesh, vertices, indices, textures);

  // Clearing lists
  ListClear(&vertices);
  ListClear(&textures);
  ListClear(&indices);
};

// Loads material textures
void LoadMaterialTextures(Model *model, List *textures,
                          struct aiMaterial *material, enum aiTextureType type,
                          char *typeName) {
  for (unsigned int i = 0; i < aiGetMaterialTextureCount(material, type); i++) {
    // Getting texture name
    aiString str;
    aiGetMaterialTexture(material, i, type, &str, NULL, NULL, NULL, NULL, NULL,
                         NULL);

    // Checking if texture is already loaded
    bool skip = false;
    for (Node *node = loadedTextures.start; node; node = node->next) {
      Texture *curTexture = (Texture *)node->data;
      if (strcmp(curTexture->path, str.data) == 0) {
        ListAppend(textures, curTexture, sizeof(Texture));
        skip = true;
        break;
      };
    };

    // Skiping if texture was already loaded
    if (skip)
      return;

    // Getting texture path in buffer
    char buffer[2048];
    int bufferSize =
        snprintf(buffer, 2048, "%s/%s", model->directory, str.data);
    if (bufferSize > sizeof(buffer) || bufferSize < 0) {
      printf("Invalid buffer size for texture path: %d", bufferSize);
      return;
    };

    // Setting up texture
    Texture texture;
    LoadTexture(GL_TEXTURE_2D, &texture.id, buffer);
    texture.type = typeName;
    texture.path = str.data;

    // Adding texture to list and to loaded textures
    ListAppend(textures, &texture, sizeof(Texture));
    ListAppend(&loadedTextures, &texture, sizeof(Texture));
  };
};
