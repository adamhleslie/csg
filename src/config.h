#ifndef CONFIG_H
#define CONFIG_H

/*
 * Global variables go here.
 */

const int kNumCirclePoints = 24;
const float kCylinderHeight = 1;
const float kCylinderRadius = .5;
const int kMaxBones = 128;
const float kError = .000000001;

/*
 * Extra credit: what would happen if you set kNear to 1e-5? How to solve it?
 */
const float kNear = 0.1f;
const float kFar = 1000.0f;
const float kFov = 45.0f;

// Floor info.
const float kFloorEps = 0.5 * (0.025 + 0.0175);
const float kFloorXMin = -100.0f;
const float kFloorXMax = 100.0f;
const float kFloorZMin = -100.0f;
const float kFloorZMax = 100.0f;
const float kFloorY = -0.75617 - kFloorEps;

#endif
