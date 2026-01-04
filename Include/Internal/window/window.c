#include "window.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "../camera/camera.h"

// Settings
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// Mouse variables
bool mouseRDown = false;
float previousX, previousY;

// -- Callback functions -- //

// Process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
void ProcessInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void FrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void ScrollCallback(GLFWwindow *window, double xOffSet, double yOffSet) {
  float radius = GetRadius() - yOffSet;

  if (radius < 1.)
    radius = 1.;

  SetRadius(radius);
}

void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    mouseRDown = true;
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    mouseRDown = false;
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos) {
  if (!mouseRDown) {
    previousX = xPos;
    previousY = yPos;
    return;
  }

  float xOffSet = xPos - previousX;
  float yOffSet = yPos - previousY;

  previousX = xPos;
  previousY = yPos;

  float pitch = GetPitch() + yOffSet / HEIGHT * M_PI;
  float yaw = GetYaw() - xOffSet / WIDTH * 2 * M_PI;

  // Clamping pitch
  pitch = glm_clamp(pitch, -M_PI / 2, M_PI / 2);

  SetPitch(pitch);
  SetYaw(yaw);
}

// glfw: Initialize and configure window
GLFWwindow *InitializeWindow(int width, int height, const char *title) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);

  if (window == NULL) {
    printf("Failed to create GLFW window");
    glfwTerminate();
    return NULL;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
  glfwSetScrollCallback(window, ScrollCallback);
  glfwSetCursorPosCallback(window, MouseCallback);
  glfwSetMouseButtonCallback(window, MouseButtonCallback);

  return window;
}
