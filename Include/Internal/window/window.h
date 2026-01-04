#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

extern const unsigned int WIDTH;
extern const unsigned int HEIGHT;

void ProcessInput(GLFWwindow *window);
GLFWwindow *InitializeWindow(int width, int height, const char *title);

#endif
