#include "./model.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

typedef struct aiFace aiFace;
typedef struct aiMaterial aiMaterial;

void ProcessNode(Model *model, struct aiNode *node,
                 const struct aiScene *scene);

void ProcessMesh(Mesh *mesh, Model *model, struct aiMesh *AImesh,
                 const struct aiScene *scene);

void LoadMaterialTextures(List *textures, struct aiMaterial *material,
                          enum aiTextureType type, char *typeName);

void DrawModel(Model *model, Shader *shader) {
  for (Node *cur = model->meshes.start; cur; cur = cur->next)
    DrawMesh(cur->data, shader);
};

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

void ProcessNode(Model *model, struct aiNode *node,
                 const struct aiScene *scene) {
  unsigned int numberMeshes = node->mNumMeshes;

  // Creating meshes struct
  List meshList;
  NewList(&meshList);

  // Adding meshes
  for (unsigned int i = 0; i < numberMeshes; i++) {
    struct aiMesh *AImesh = scene->mMeshes[node->mMeshes[i]];

    Mesh mesh;
    ProcessMesh(&mesh, model, AImesh, scene);
    ListAppend(&meshList, &mesh, sizeof(Mesh));
  };

  // Processing children (recursion)
  unsigned int numberChildren = node->mNumMeshes;
  for (unsigned int i = 0; i < numberChildren; i++) {
    ProcessNode(model, node->mChildren[i], scene);
  };
};

void ProcessMesh(Mesh *mesh, Model *model, struct aiMesh *AImesh,
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
    memcpy(vertex.Position, vector, 3 * sizeof(float));

    // Getting normals
    vector[0] = AImesh->mNormals[i].x;
    vector[1] = AImesh->mNormals[i].y;
    vector[2] = AImesh->mNormals[i].z;
    memcpy(vertex.Normal, vector, 3 * sizeof(float));

    // Getting texture coordinates
    if (AImesh->mTextureCoords[0]) {
      vec2 vector_;
      vector_[0] = AImesh->mTextureCoords[0][i].x;
      vector_[1] = AImesh->mTextureCoords[0][i].y;
      memcpy(vertex.TexCoord, vector_, 2 * sizeof(float));
    } else
      memcpy(vertex.TexCoord, (vec2){0., 0.}, 2 * sizeof(float));

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
    LoadMaterialTextures(&diffuseMaps, material, aiTextureType_DIFFUSE,
                         "texture_diffuse");
    // textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    LoadMaterialTextures(&specularMaps, material, aiTextureType_SPECULAR,
                         "texture_specular");
    // textures.insert(textures.end(), specularMaps.begin(),
    // specularMaps.end());
  };

  NewMesh(mesh, vertices, indices, textures);
};
