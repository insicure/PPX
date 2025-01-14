#pragma once

#include "Vec2.hpp"
#include "f32.hpp"

namespace ppx
{
  class Rect {
  public:
    f32 x;
    f32 y;
    f32 width;
    f32 height;

    constexpr Rect()                           : x(), y(), width(), height() { }
    constexpr Rect(f32 x, f32 y, f32 w, f32 h) : x(x), y(y), width(w), height(h) { }
    constexpr Rect(f32 w, f32 h)               : x(), y(), width(w), height(h) { }

    inline f32 Left() const   { return x; }
    inline f32 Right() const  { return x+width; }
    inline f32 Top() const    { return y; }
    inline f32 Bottom() const { return y+height; }

    inline Vec2 Min() const   { return Vec2(Left(), Top()); }
    inline Vec2 Max() const   { return Vec2(Right(), Bottom()); }
  };
}