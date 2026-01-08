#ifndef SHADER_H
#define SHADER_H

#include "../../External/cglm/include/cglm/cglm.h"

#include <stdbool.h>

typedef struct {
  unsigned int ID;
} Shader;

// Shader constructor
Shader *NewShader(const char *vertexPath, const char *fragmentPath);

// Use shader
void UseShader(Shader *shader);

// Utility functions
void SetBool(Shader *shader, const char *name, bool value);
void SetInt(Shader *shader, const char *name, int value);
void SetFloat(Shader *shader, const char *name, float value);
void SetMat4(Shader *shader, const char *name, mat4 value);
void SetMat3(Shader *shader, const char *name, mat3 value);
void SetVec3(Shader *shader, const char *name, vec3 value);

#endif
