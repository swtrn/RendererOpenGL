#include "Include/cglm/include/cglm/cglm.h"
#include "Include/glad/glad.h"
#include "Include/shader/shader.h"
#include "Include/stb_image/stb_image.h"

#include <GLFW/glfw3.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifdef __APPLE__
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// TODO: Organize code into more functions to be properly used in other
//       projects.
// TODO: Add camera movement.

unsigned int VBO, VAO, EBO;      // Vertex objects
unsigned int texture1, texture2; // Texture
Shader *shader;                  // Shader

vec3 cubePositions[] = {{0.0f, 0.0f, 0.0f},    {2.0f, 5.0f, -15.0f},
                        {-1.5f, -2.2f, -2.5f}, {-3.8f, -2.0f, -12.3f},
                        {2.4f, -0.4f, -3.5f},  {-1.7f, 3.0f, -7.5f},
                        {1.3f, -2.0f, -2.5f},  {1.5f, 2.0f, -2.5f},
                        {1.5f, 0.2f, -1.5f},   {-1.3f, 1.0f, -1.5f}};

// The function below is called whenever user resizes window.
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// Process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// Loads a texture
void LoadTexture(int target, unsigned int *texture, char *texturePath) {
  // Creating and binding textures
  glGenTextures(1, texture);
  glBindTexture(target, *texture);

  // Loading image
  int width, height, numberChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(texturePath, &width, &height, &numberChannels,
                                  0); // Triangle vertices
  if (data) {
    // Generating texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("Failed to load texture.");
  }

  // Freeing allocated image data
  stbi_image_free(data);
}

// Binds Textures
void UseTexture(int texture, int index, int target) {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(target, texture);
}

// glfw: Initialize and configure window
GLFWwindow *InitializeWindow(int width, int height, const char *title) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw window creation
  GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);

  if (window == NULL) {
    printf("Failed to create GLFW window");
    glfwTerminate();
    return NULL;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  return window;
}

// glad: Load all OpenGL function pointers
bool LoadOpenGL() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD");
    return false;
  }

  return true;
}

// Runs every frame
void Update(GLFWwindow *window) {
  // Input
  processInput(window);

  // Render
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Matrices
  mat4 view, projection;

  glm_mat4_identity(view);

  glm_translate(view, (vec3){0., 0., -3.});
  glm_perspective(glm_rad(45.), 800. / 600., 0.1, 100., projection);

  // Sending projection and view matrices to shader
  char *varNames[] = {"view", "projection"};
  float *matPointers[] = {*view, *projection};
  for (int i = 0; i < 2; i++)
    SetMat4(shader, varNames[i], matPointers[i]);

  UseTexture(texture1, 0, GL_TEXTURE_2D);
  UseTexture(texture2, 1, GL_TEXTURE_2D);

  glBindVertexArray(VAO);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  // Drawing many cubes
  for (int i = 0; i < 10; i++) {
    mat4 model;
    glm_mat4_identity(model);

    // Adding rotation and translation
    glm_translate(model, cubePositions[i]);
    float angle = 20. * (i + 1) * (float)glfwGetTime();
    glm_rotate(model, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});

    SetMat4(shader, "model", *model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

  // Swap buffers and poll IO events (keys pressed/released, etc)
  glfwSwapBuffers(window);
  glfwPollEvents();
}

int main() {
  // Initializing window
  GLFWwindow *window;
  if ((window = InitializeWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL")) == NULL)
    return -1;

  // Loading OpenGL pointers
  if (!LoadOpenGL())
    return -1;

  // --- Graphics Shader --- //

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

  // float vertices[] = {
  //     // positions        // colors         // texture coords
  //     0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
  //     0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
  //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
  //     -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  // };

  // unsigned int indices[] = {
  //     0, 1, 3, // First triangle
  //     1, 2, 3  // Second triangle
  // };

  // Initializing buffer and vertex array object
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // glGenBuffers(1, &EBO);

  // Binding array object
  glBindVertexArray(VAO);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  //              GL_STATIC_DRAW);

  // Copies and binds *vertices* to buffer memory
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // TODO: change pointer and stride parameters when adding color back

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Color attribute
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
  //                       (void *)(3 * sizeof(float)));
  // glEnableVertexAttribArray(1);

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

  // Render loop
  while (!glfwWindowShouldClose(window))
    Update(window);

  // Deleting arrays, buffers and programs.
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  // glDeleteBuffers(1, &EBO);
  glDeleteProgram(shader->ID);
  free(shader);

  // Clearing everything
  glfwTerminate();
  return 0;
}
