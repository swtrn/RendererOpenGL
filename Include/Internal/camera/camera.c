#include "camera.h"

#include "../../Internal/window/window.h"

#include <math.h>

// Constant
const float FOV = 90.;

// Variables
float cameraPosition[3];

// Camera variables
vec3 pivotPoint = {0., 0., 0.};
float sphereRadius;
float yaw, pitch;

// Pitch
float GetPitch() { return pitch; }
void SetPitch(float theta) {
  pitch = theta;

  // Updating camera position
  cameraPosition[1] = sin(pitch) * sphereRadius;
}

// Yaw
float GetYaw() { return yaw; }
void SetYaw(float theta) {
  yaw = theta;

  // Updating camera position
  cameraPosition[0] = sin(yaw) * sphereRadius;
  cameraPosition[2] = cos(yaw) * sphereRadius;
}

// Radius
float GetRadius() { return sphereRadius; }
void SetRadius(float radius) {
  sphereRadius = radius;

  // Updating camera position
  cameraPosition[0] = sin(yaw) * sphereRadius;
  cameraPosition[2] = cos(yaw) * sphereRadius;
  cameraPosition[1] = sin(pitch) * sphereRadius;
}

// -- Matrices -- //

void GetViewMatrix(mat4 view) {
  glm_lookat(cameraPosition, pivotPoint, (vec3){0., 1., 0.}, view);
}

void GetProjectionMatrix(mat4 projection) {
  glm_perspective(glm_rad(FOV), (float)WIDTH / HEIGHT, 0.1, 100., projection);
}
