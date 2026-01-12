#include "Include/External/glad/glad.h"

#include "Include/Internal/camera/camera.h"
#include "Include/Internal/shader/shader.h"
#include "Include/Internal/texture/texture.h"
#include "Include/Internal/window/window.h"

#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>

GLFWwindow *window;   // Window
Shader *objectShader; // Lightning shader
Shader *lightShader;  // Light itself shader

// Vertices
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
    0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
    0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
    -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
    -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
    -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
    -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};

// Positions
vec3 cubePositions[] = {{0.0f, 0.0f, 0.0f},    {2.0f, 5.0f, -15.0f},
                        {-1.5f, -2.2f, -2.5f}, {-3.8f, -2.0f, -12.3f},
                        {2.4f, -0.4f, -3.5f},  {-1.7f, 3.0f, -7.5f},
                        {1.3f, -2.0f, -2.5f},  {1.5f, 2.0f, -2.5f},
                        {1.5f, 0.2f, -1.5f},   {-1.3f, 1.0f, -1.5f}};

vec3 lightPositions[] = {{0.7f, 0.2f, 2.0f},
                         {2.3f, -3.3f, -4.0f},
                         {-4.0f, 2.0f, -12.0f},
                         {0.0f, 0.0f, -3.0f}};

unsigned int VBO, VAO, EBO;           // Vertex objects
unsigned int diffuseMap, specularMap; // Texture

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

  SetProjection(objectShader);

  // Enabling textures
  UseTexture(diffuseMap, 0, GL_TEXTURE_2D);
  UseTexture(specularMap, 1, GL_TEXTURE_2D);

  // Material
  SetFloat(objectShader, "material.shine", 64.f);

  // -- Lights -- //

  // Directional light
  SetVec3(objectShader, "directionalLight.direction", (vec3){-0.2, -1.0, -0.3});
  SetVec3(objectShader, "directionalLight.ambient", (vec3){0.05, 0.05, 0.05});
  SetVec3(objectShader, "directionalLight.diffuse", (vec3){0.4, 0.4, 0.4});
  SetVec3(objectShader, "directionalLight.specular", (vec3){0.5, 0.5, 0.5});

  // Point lights
  for (unsigned int i = 0; i < 4; i++) {
    char buffer[64];

    // Setting vector light parameters
    for (int j = 0; j < 4; j++) {
      char *texts[] = {"ambient", "diffuse", "specular", "position"};
      vec3 vectors[4] = {{.2f, .2f, .2f}, {.5f, .5f, .5f}, {1.0f, 1.0f, 1.0f}};
      glm_vec3_dup(lightPositions[i], vectors[3]);

      int size =
          snprintf(buffer, sizeof(buffer), "pointLights[%d].%s", i, texts[j]);

      if (size < 0 || size > sizeof(buffer)) {
        printf("Invalid buffer size: %d", size);
        exit(1);
      };

      SetVec3(objectShader, buffer, vectors[j]);
    };

    // Setting float light parameters
    for (int j = 0; j < 3; j++) {
      char *texts[] = {"constant", "linear", "quadratic"};
      float values[] = {1.0, 0.09, 0.032};

      int size =
          snprintf(buffer, sizeof(buffer), "pointLights[%d].%s", i, texts[j]);

      if (size < 0 || size > sizeof(buffer)) {
        printf("Invalid buffer size: %d", size);
        exit(1);
      };

      SetFloat(objectShader, buffer, values[j]);
    };
  };

  SetVec3(objectShader, "viewPosition", cameraPosition);

  mat4 model; // Model matrix placeholder

  // Setting cube positions
  for (unsigned int i = 0; i < 10; i++) {
    // Model matrix
    SetTransform(model, cubePositions[i], glm_rad(20. * i), (vec3){1., .3, .5},
                 (vec3){1., 1., 1.});

    // Normal matrix
    mat4 normalMatrix4x4;
    mat3 normalMatrix;

    glm_mat4_inv(model, normalMatrix4x4);
    glm_mat4_pick3t(normalMatrix4x4, normalMatrix);

    // Setting matrices
    SetMat4(objectShader, "model", model);
    SetMat3(objectShader, "normalMatrix", normalMatrix);

    // Bind and draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

  // Setting lightShader
  UseShader(lightShader);

  SetProjection(lightShader);

  // Setting light cube positions
  for (int i = 0; i < 4; i++) {
    // Model matrix
    SetTransform(model, lightPositions[i], 0., GLM_VEC3_ONE,
                 (vec3){.2, .2, .2});
    SetMat4(lightShader, "model", model);

    // Bind and draw
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

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
  const char *objectFragmentPath = "./Shaders/Textured/multipleCasters.glsl";
  const char *objectVertexPath = "./Shaders/Textured/objectVertex.glsl";

  const char *lightFragmentPath = "./Shaders/Light/lightFragment.glsl";
  const char *lightVertexPath = "./Shaders/Light/lightVertex.glsl";

  // Generating shaders
  objectShader = NewShader(objectVertexPath, objectFragmentPath);
  lightShader = NewShader(lightVertexPath, lightFragmentPath);

  // --- Textures --- //

  // Loading textures
  LoadTexture(GL_TEXTURE_2D, &diffuseMap, "./Images/container.png");
  LoadTexture(GL_TEXTURE_2D, &specularMap, "./Images/containerSpecular.png");

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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Normals attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // -- light VAO -- //

  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Unbinding VAO and clearing VBO
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Setting shader textures
  UseShader(objectShader);

  SetInt(objectShader, "material.diffuse", 0);
  SetInt(objectShader, "material.specular", 1);

  glEnable(GL_DEPTH_TEST);

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
