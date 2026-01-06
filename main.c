#include "Include/External/glad/glad.h"

#include "Include/Internal/camera/camera.h"
#include "Include/Internal/shader/shader.h"
#include "Include/Internal/texture/texture.h"
#include "Include/Internal/window/window.h"

#include <stdbool.h>
#include <stdio.h>

GLFWwindow *window; // Window
Shader *shader;     // Shader

// Vertices
float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

// Cube positions
vec3 cubePositions[] = {{0.0f, 0.0f, 0.0f},    {2.0f, 5.0f, -15.0f},
                        {-1.5f, -2.2f, -2.5f}, {-3.8f, -2.0f, -12.3f},
                        {2.4f, -0.4f, -3.5f},  {-1.7f, 3.0f, -7.5f},
                        {1.3f, -2.0f, -2.5f},  {1.5f, 2.0f, -2.5f},
                        {1.5f, 0.2f, -1.5f},   {-1.3f, 1.0f, -1.5f}};

unsigned int VBO, VAO, EBO;      // Vertex objects
unsigned int texture1, texture2; // Texture

// glad: Load all OpenGL function pointers
bool LoadOpenGL() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD");
    return false;
  }

  return true;
}

void SetProjection() {
  // Matrices
  mat4 view, projection;

  GetViewMatrix(view);
  GetProjectionMatrix(projection);

  // Sending projection and view matrices to shader
  char *varNames[] = {"view", "projection"};
  float *matPointers[] = {*view, *projection};
  for (int i = 0; i < 2; i++)
    SetMat4(shader, varNames[i], matPointers[i]);
}

// Runs every frame
void Update() {
  // Input
  ProcessInput(window);

  // Render
  glClearColor(0.2, 0.2, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Setting camera projection
  SetProjection();

  // Enabling textures
  UseTexture(texture1, 0, GL_TEXTURE_2D);
  UseTexture(texture2, 1, GL_TEXTURE_2D);

  glBindVertexArray(VAO);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  // Initializing model matrix
  mat4 model;
  glm_mat4_identity(model);

  // Adding translation
  glm_translate(model, cubePositions[0]);

  // Adding rotation
  float angle = 0.;
  glm_rotate(model, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});

  SetMat4(shader, "model", *model);

  // Draw
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
  const char *vertexPath = "./Shaders/vertexShader.glsl";
  const char *fragmentPath = "./Shaders/fragmentShader.glsl";

  // Generating shader
  shader = NewShader(vertexPath, fragmentPath);

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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Setting shader, textures and functionalities
  UseShader(shader);
  SetInt(shader, "texture1", 0);
  SetInt(shader, "texture2", 1);

  glEnable(GL_DEPTH_TEST);

  // Initializing camera
  SetRadius(4.);

  // Render loop
  while (!glfwWindowShouldClose(window))
    Update();

  // Deleting arrays, buffers and programs.
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader->ID);
  free(shader);

  // Clearing everything
  glfwTerminate();
  return 0;
}
