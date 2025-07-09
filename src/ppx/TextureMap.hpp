#pragma once

#include "Color.hpp"
#include "Rect.hpp"
#include "Vec2.hpp"
#include <cstdint>

namespace ppx
{
  class TextureMap
  {
  public:
    int id = 0;
    uint16_t width = 0;
    uint16_t height = 0;
    uint32_t hash = 0;
    int16_t frame_x = 0;
    int16_t frame_y = 0;
    int16_t offset_x = 0;
    int16_t offset_y = 0;
    int16_t frame_width = 0;
    int16_t frame_height = 0;
    bool rotated = false;

    void Draw(const Vec2 &position, 
              const Vec2 &scale = {1, 1},
              const Vec2 &origin = {0, 0},
              int rotation = 0,
              bool flip_x = false, bool flip_y = false,
              const Rect &region = {0, 0, -1, -1},
              const Color tint = 0xffffff);
  };
}