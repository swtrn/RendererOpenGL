#include "../../External/cglm/include/cglm/cglm.h"

#ifndef CAMERA_H
#define CAMERA_H

extern const float FOV;

float GetPitch();
void SetPitch(float theta);

float GetYaw();
void SetYaw(float theta);

float GetRadius();
void SetRadius(float radius);

void GetViewMatrix(mat4 view);
void GetProjectionMatrix(mat4 projection);

#endif
