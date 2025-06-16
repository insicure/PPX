#pragma once

#include "Texture.hpp"

namespace ppx
{
  class TextureMap : public Texture
  {
  public:
    uint32_t hash = 0;
    int16_t frame_x = 0;
    int16_t frame_y = 0;
    int16_t offset_x = 0;
    int16_t offset_y = 0;
    int16_t frame_width = 0;
    int16_t frame_height = 0;
    bool rotated = false;

    constexpr TextureMap()
      : Texture(), hash(0), frame_x(0), frame_y(0), offset_x(0), offset_y(0), frame_width(0), frame_height(0), rotated(false) { }

    void Unload() override;
    bool isValid() override;

    void Draw(const Vec2 &position, 
                      const Vec2 &scale = {1, 1},
                      const Vec2 &origin = {0, 0},
                      int rotation = 0,
                      bool flip_x = false, bool flip_y = false,
                      const Rect &region = {0, 0, -1, -1}) override;
  };
}