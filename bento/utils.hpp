#pragma once

#include "bento/struct.hpp"
namespace nb {

  void TraceLog(const char *format, ...);

  Vec2 GetCameraToScreen(const Vec2 &position, const Camera &camera);
  Vec2 GetScreenToCamera(const Vec2 &position, const Camera &camera);
  Matrix GetCameraMatrix(const Camera &camera);

}