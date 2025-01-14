#pragma once

#include "Matrix.hpp"
#include "Vec2.hpp"

namespace ppx
{
  class Camera {
  public:
    Vec2 offset;
    Vec2 target;
    int16_t rotation;
    f32 zoom;

    constexpr Camera() : offset(), target(), rotation(0), zoom(1) { }

    Vec2 CameraToScreen(const Vec2 &position);
    Vec2 ScreenToScreen(const Vec2 &position);
    Matrix GetMatrix();
  };
}