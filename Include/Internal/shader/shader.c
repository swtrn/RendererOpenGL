#include "./shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../External/glad/glad.h"

// Utility function for checking shader compilation/linking errors.
void CheckCompileErrors(unsigned int shader, char *type) {
  int success;
  char infoLog[1024];

  if (strcmp(type, "PROGRAM")) {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type,
             infoLog);
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n", type, infoLog);
    }
  }
}

// Read file
char *ReadFile(const char *filePath) {
  // Getting stream
  FILE *file = fopen(filePath, "r");
  if (file == NULL) {
    printf("Error while opening vertex path.");
    fclose(file);
  }

  // Getting length of file
  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = malloc(length + 1);

  // Writing to buffer
  size_t elements = fread(buffer, 1, length, file);
  if (elements < length)
    printf("Error while reading vertex path.");
  fclose(file);

  // Terminating string
  buffer[length] = 0;

  return buffer;
};

// Shader constructor
Shader *NewShader(const char *vertexPath, const char *fragmentPath) {
  // Reading files
  char *vertexCode = ReadFile(vertexPath);
  char *fragmentCode = ReadFile(fragmentPath);

  // Assigning to const
  const char *vertexCodeConst = vertexCode;
  const char *fragmentCodeConst = fragmentCode;

  // Compiling shaders

  unsigned int vertex, fragment;
  char infoLog[512];
  int success;

  // Vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexCodeConst, NULL);
  glCompileShader(vertex);
  CheckCompileErrors(vertex, "VERTEX");

  // Fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragmentCodeConst, NULL);
  glCompileShader(fragment);
  CheckCompileErrors(fragment, "FRAGMENT");

  // Shader program
  Shader *shader = (Shader *)malloc(sizeof(Shader));
  shader->ID = glCreateProgram();

  glAttachShader(shader->ID, vertex);
  glAttachShader(shader->ID, fragment);
  glLinkProgram(shader->ID);

  // Print linking errors if any
  CheckCompileErrors(shader->ID, "PROGRAM");

  // Deleting the shaders and freeing memory
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  free(vertexCode);
  free(fragmentCode);

  // Returning pointer
  return shader;
};

// Use function
void UseShader(Shader *shader) { glUseProgram(shader->ID); };

// Utility functions

void SetBool(Shader *shader, const char *name, bool value) {
  glUniform1i(glGetUniformLocation(shader->ID, name), (int)value);
};

void SetInt(Shader *shader, const char *name, int value) {
  glUniform1i(glGetUniformLocation(shader->ID, name), value);
};

void SetFloat(Shader *shader, const char *name, float value) {
  glUniform1f(glGetUniformLocation(shader->ID, name), value);
};

void SetMat4(Shader *shader, const char *name, mat4 value) {
  int uniformLocation = glGetUniformLocation(shader->ID, name);
  glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, *value);
};

void SetMat3(Shader *shader, const char *name, mat3 value) {
  int uniformLocation = glGetUniformLocation(shader->ID, name);
  glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, *value);
};

void SetVec3(Shader *shader, const char *name, vec3 value) {
  int uniformLocation = glGetUniformLocation(shader->ID, name);
  glUniform3fv(uniformLocation, 1, &value[0]);
};
