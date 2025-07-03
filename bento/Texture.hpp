#pragma once

#include "SillyImage.hpp"
#include "Rect.hpp"
#include "Vec2.hpp"
#include "Color.hpp"

namespace ppx
{
  class Texture {
  public:
    int id = 0;
    uint16_t width = 0;
    uint16_t height = 0;

    virtual ~Texture() { }

    static Texture *Load(const SillyImage &image);
    static Texture *Load(const char *filename);
    virtual void Unload();
    virtual bool isValid();

    virtual void Draw(const Vec2 &position, 
                      const Vec2 &scale = {1, 1},
                      const Vec2 &origin = {0, 0},
                      int rotation = 0,
                      bool flip_x = false, bool flip_y = false,
                      const Rect &region = {0, 0, -1, -1},
                      const Color tint = 0xffffff);
    protected:
      Texture() = default;
      Texture& operator=(const Texture&) = delete;
  };
}