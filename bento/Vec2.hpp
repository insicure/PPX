#pragma once

#include "f32.hpp"

namespace ppx
{
  class Vec2 {
  public:
    f32 x, y;

    constexpr Vec2()                 : x(), y() { }
    constexpr Vec2(f32 x, f32 y)     : x(x), y(y) { }
  };

  static inline Vec2 operator*(const Vec2 &lhs, const f32 &rhs)   { return Vec2(lhs.x * rhs, lhs.y * rhs); }
  static inline Vec2 operator/(const Vec2 &lhs, const f32 &rhs)   { return Vec2(lhs.x / rhs, lhs.y / rhs); }
  static inline Vec2 operator+(const Vec2 &lhs, const f32 &rhs)   { return Vec2(lhs.x + rhs, lhs.y + rhs); }
  static inline Vec2 operator-(const Vec2 &lhs, const f32 &rhs)   { return Vec2(lhs.x - rhs, lhs.y - rhs); }

  static inline Vec2 &operator*=(Vec2 &lhs, const f32 &rhs)       { lhs.x = lhs.x * rhs, lhs.y = lhs.y * rhs; return lhs; }
  static inline Vec2 &operator/=(Vec2 &lhs, const f32 &rhs)       { lhs.x = lhs.x / rhs, lhs.y = lhs.y / rhs; return lhs; }
  static inline Vec2 &operator+=(Vec2 &lhs, const f32 &rhs)       { lhs.x = lhs.x + rhs, lhs.y = lhs.y + rhs; return lhs; }
  static inline Vec2 &operator-=(Vec2 &lhs, const f32 &rhs)       { lhs.x = lhs.x - rhs, lhs.y = lhs.y - rhs; return lhs; }

  static inline bool operator==(const Vec2 &lhs, const Vec2 &rhs)    { return (lhs.x == rhs.x) && (lhs.y == rhs.y); }
  static inline bool operator!=(const Vec2 &lhs, const Vec2 &rhs)    { return (lhs.x != rhs.x) && (lhs.y != rhs.y); }
}