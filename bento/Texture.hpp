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
    int id = 0;
    uint16_t width = 0;
    uint16_t height = 0;

    constexpr Texture() : id(0), width(0), height(0) {}

    Texture(const SillyImage &image);
    Texture(const char *filename);
    virtual ~Texture() { }

    bool Load(const SillyImage &image);
    bool Load(const char *filename);
    virtual void Unload();
    virtual bool isValid();

    virtual void Draw(const Vec2 &position, const int transform = 0) const;
    virtual void Draw(const Vec2 &position, const int transform, const int rotation, const Vec2 &scale, const Vec2 origin) const;
    virtual void Draw(const Rect &region, const Rect &dest, const int transform, const int rotation, const Vec2 &origin) const;
  };
}