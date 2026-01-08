#include "Include/External/glad/glad.h"

#include "Include/Internal/camera/camera.h"
#include "Include/Internal/shader/shader.h"
#include "Include/Internal/texture/texture.h"
#include "Include/Internal/window/window.h"

#include <stdbool.h>
#include <stdio.h>

GLFWwindow *window;   // Window
Shader *objectShader; // Lightning shader
Shader *lightShader;  // Light itself shader

// Vertices
float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f,
    0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f,
    0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,
    0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f,
    0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

// Positions
vec3 cubePosition = {0.0f, 0.0f, 0.0f};
vec3 lightPosition = {1.2f, 1.0f, 2.0f};

// Colors
vec3 objectColor = {1., .5, .32};
vec3 lightColor = {1., 1., 1.};

unsigned int VBO, VAO, EBO;      // Vertex objects
unsigned int texture1, texture2; // Texture

// Light VAO
unsigned int lightVAO;

// glad: Load all OpenGL function pointers
bool LoadOpenGL() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD");
    return false;
  }

  return true;
}

void SetProjection(Shader *shader) {
  // Matrices
  mat4 view, projection;

  GetViewMatrix(view);
  GetProjectionMatrix(projection);

  // Sending projection and view matrices to shader
  char *varNames[] = {"view", "projection"};
  mat4 *matPointers[] = {&view, &projection};
  for (int i = 0; i < 2; i++)
    SetMat4(shader, varNames[i], *matPointers[i]);
}

void SetTransform(mat4 matrix, vec3 position, float angle, vec3 rotationAxis,
                  vec3 scale) {
  glm_mat4_identity(matrix);
  glm_translate(matrix, position);
  glm_rotate(matrix, angle, rotationAxis);
  glm_scale(matrix, scale);
}

// Runs every frame
void Update() {
  // Input
  ProcessInput(window);

  // Render
  glClearColor(0.2, 0.2, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Setting camera
  vec3 cameraPosition;
  GetCameraPosition(cameraPosition);

  // Setting objectShader
  UseShader(objectShader);

  SetVec3(objectShader, "objectColor", objectColor);
  SetVec3(objectShader, "lightColor", lightColor);

  SetVec3(objectShader, "lightPosition", lightPosition);
  SetVec3(objectShader, "viewPosition", cameraPosition);

  // Enabling textures
  UseTexture(texture1, 0, GL_TEXTURE_2D);
  UseTexture(texture2, 1, GL_TEXTURE_2D);

  // Setting matrices

  mat4 model; // Matrix holder

  // Model matrix
  SetTransform(model, cubePosition, glm_rad(20. * glfwGetTime()),
               (vec3){1., .3, .5}, (vec3){1., 1., 1.});

  SetMat4(objectShader, "model", model);
  SetProjection(objectShader);

  // Normal matrix
  mat4 normalMatrix4x4;
  mat3 normalMatrix;

  glm_mat4_inv(model, normalMatrix4x4);
  glm_mat4_pick3t(normalMatrix4x4, normalMatrix);

  SetMat3(objectShader, "normalMatrix", normalMatrix);

  // Bind and draw
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Setting lightShader
  UseShader(lightShader);

  // Setting matrices
  SetTransform(model, lightPosition, 0., (vec3){0, 1., 0}, (vec3){.2, .2, .2});

  SetMat4(lightShader, "model", model);
  SetProjection(lightShader);

  // Bind and draw
  glBindVertexArray(lightVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Swap buffers and poll IO events (keys pressed/released, etc)
  glfwSwapBuffers(window);
  glfwPollEvents();
}

// Main
int main() {
  // Initializing window
  if ((window = InitializeWindow(WIDTH, HEIGHT, "LearnOpenGL")) == NULL)
    return -1;

  // Loading OpenGL pointers
  if (!LoadOpenGL())
    return -1;

  // --- Graphics Shader --- //

  // Shader paths
  const char *objectFragmentPath = "./Shaders/objectFragment.glsl";
  const char *objectVertexPath = "./Shaders/objectVertex.glsl";
  const char *lightFragmentPath = "./Shaders/lightFragment.glsl";
  const char *lightVertexPath = "./Shaders/lightVertex.glsl";

  // Generating shaders
  objectShader = NewShader(objectVertexPath, objectFragmentPath);
  lightShader = NewShader(lightVertexPath, lightFragmentPath);

  // --- Textures --- //

  // Texture wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  // Texture filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);

  // Loading textures
  LoadTexture(GL_TEXTURE_2D, &texture1, "./miku.jpg");
  LoadTexture(GL_TEXTURE_2D, &texture2, "./furina.jpg");

  // --- Buffers --- //

  // Initializing buffer and vertex array object
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // Binding array object
  glBindVertexArray(VAO);

  // Copies and binds *vertices* to buffer memory
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Normals attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture attribute
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
  //                       (void *)(3 * sizeof(float)));
  // glEnableVertexAttribArray(2);

  // -- Light VAO -- //

  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Unbinding VAO and clearing VBO
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glEnable(GL_DEPTH_TEST);

  // Setting shader textures
  UseShader(objectShader);

  SetInt(objectShader, "texture1", 0);
  SetInt(objectShader, "texture2", 1);

  // Initializing camera
  SetRadius(4.);

  // Render loop
  while (!glfwWindowShouldClose(window))
    Update();

  // Deleting arrays, buffers and programs.
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glDeleteProgram(objectShader->ID);
  glDeleteProgram(lightShader->ID);

  free(objectShader);
  free(lightShader);

  // Clearing everything
  glfwTerminate();
  return 0;
}
