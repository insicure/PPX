#pragma once

#include "Vec2.hpp"
#include "f32.hpp"

namespace ppx
{
  class Rect {
  public:
    f32 x{};
    f32 y{};
    f32 width{};
    f32 height{};

    constexpr Rect() = default;
    constexpr Rect(f32 x, f32 y, f32 w, f32 h) : x(x), y(y), width(w), height(h) { }
    constexpr Rect(f32 w, f32 h)               : x(0), y(0), width(w), height(h) { }

    f32 Left() const   { return x; }
    f32 Right() const  { return x+width; }
    f32 Top() const    { return y; }
    f32 Bottom() const { return y+height; }

    Vec2 Min() const   { return Vec2(Left(), Top()); }
    Vec2 Max() const   { return Vec2(Right(), Bottom()); }
  };
}