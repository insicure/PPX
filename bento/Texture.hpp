#pragma once

#include "SillyImage.hpp"
#include "Rect.hpp"
#include "Vec2.hpp"
#include "Color.hpp"

namespace ppx
{
  struct Texture {
  public:
    int id = 0;
    uint16_t width = 0;
    uint16_t height = 0;

    void Draw(const Vec2 &position, 
              const Vec2 &scale = {1, 1},
              const Vec2 &origin = {0, 0},
              int rotation = 0,
              bool flip_x = false, bool flip_y = false,
              const Rect &region = {0, 0, -1, -1},
              const Color tint = 0xffffff);
  };

  Texture *Load_Texture(const SillyImage &image);
  Texture *Load_Texture(const char *filename);
  void Unload_Texture(Texture *&ptr);
}