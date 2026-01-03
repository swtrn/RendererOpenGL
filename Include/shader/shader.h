#ifndef SHADER_H
#define SHADER_H

#include "../glad/glad.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void SetMat4(Shader *shader, const char *name, float *pointer);

#endif
