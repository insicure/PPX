#pragma once

#include "SillyImage.hpp"
#include "Rect.hpp"
#include "Vec2.hpp"

namespace ppx
{
  
  #define TRANS_NONE    0x00
  #define TRANS_FLIP_X  0x01
  #define TRANS_FLIP_Y  0x02

  class Texture {
  public:
    int id;
    uint16_t width;
    uint16_t height;

    constexpr Texture() : id(0), width(0), height(0) {}

    Texture(const SillyImage &image);
    Texture(const char *filename);

    int Load(const SillyImage &image);
    int Load(const char *filename);
    virtual void Unload();
    virtual bool isValid();

    virtual void Draw(const Vec2 &position, const int transform);
    virtual void Draw(const Vec2 &position, const int transform, const int rotation, const Vec2 &scale, const Vec2 origin);
    virtual void Draw(const Rect &region, const Rect &dest, const int transform, const int rotation, const Vec2 &origin);
  };
}