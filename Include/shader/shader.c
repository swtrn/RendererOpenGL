#include "./shader.h"

// Shader constructor
Shader *NewShader(const char *vertexPath, const char *fragmentPath) {
  // Allocatting strings
  char *vertexCode;
  char *fragmentCode;

  // Allocatting files
  FILE *vertexPtr;
  FILE *fragmentPtr;

  int ch; // Char int holder

  // Open vertex file in read mode
  vertexPtr = fopen(vertexPath, "r");
  if (vertexPtr == NULL) {
    printf("Failed to open vertex path.");
    fclose(vertexPtr);
  }

  // Reading file and closing
  while ((ch = fgetc(vertexPtr)) != EOF)
    *vertexCode++ = ch;

  *vertexCode = '\0';

  fclose(vertexPtr);

  printf("%s\nEOF\n", vertexCode);

  // Assigning to const
  const char *vertexCodeConst = vertexCode;

  // Open fragment file in read mode
  fragmentPtr = fopen(fragmentPath, "r");
  if (fragmentPtr == NULL) {
    printf("Failed to open fragment path.");
    fclose(fragmentPtr);
  }

  // Reading file and closing
  while ((ch = fgetc(fragmentPtr)) != EOF)
    *fragmentCode++ = ch;

  *fragmentCode = '\0';

  fclose(fragmentPtr);

  printf("%s\nEOF\n", fragmentCode);

  // Assigning to const
  const char *fragmentCodeConst = fragmentCode;

  // Compiling shaders

  unsigned int vertex, fragment;
  char infoLog[512];
  int success;

  // Vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexCodeConst, NULL);
  glCompileShader(vertex);

  // Print compile errors if any
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
  };

  // Fragment Shader
  fragment = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(fragment, 1, &fragmentCodeConst, NULL);
  glCompileShader(fragment);

  // Print compile errors if any
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
  };

  // Shader program
  Shader *shader = (Shader *)malloc(sizeof(Shader));
  unsigned int ID = glCreateProgram();
  shader->ID = ID;

  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  // Print linking errors if any
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
  }

  // Deleting the shaders
  glDeleteShader(vertex);
  glDeleteShader(fragment);

  // Returning pointer
  return shader;
};

// Use function
void Use(Shader *shader) { glUseProgram(shader->ID); };

// Utility functions

void SetBool(Shader *shader, const char *name, bool value) {
  glUniform1i(glGetUniformLocation(shader->ID, name), (int)value);
};

void SetInt(Shader *shader, const char *name, int value) {
  glUniform1i(glGetUniformLocation(shader->ID, name), value);
};

void SetFloat(Shader *shader, const char *name, float value) {
  glUniform1i(glGetUniformLocation(shader->ID, name), value);
};
