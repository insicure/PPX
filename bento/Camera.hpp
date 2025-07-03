#pragma once

#include "Matrix.hpp"
#include "Vec2.hpp"
#include <cstdint>

namespace ppx
{
  class Camera {
  public:
    Vec2 offset{};
    Vec2 target{};
    int16_t rotation = 0;
    f32 zoom = 1;

    constexpr Camera() : offset(0, 0), target(0, 0), rotation(0), zoom(1) { }

    void Set(const Vec2 offset, const Vec2 target, const int16_t rotation, const f32 zoom);
    Vec2 CameraToScreen(const Vec2 position);
    Vec2 ScreenToScreen(const Vec2 position);
    Matrix GetMatrix();
  };
}