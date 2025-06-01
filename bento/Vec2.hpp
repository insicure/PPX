#pragma once

#include "f32.hpp"

namespace ppx
{
  class Vec2 {
  public:
    f32 x{};
    f32 y{};

    constexpr Vec2() = default;
    constexpr Vec2(f32 x, f32 y)     : x(x), y(y) { }
  };

  static inline Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs) { return {lhs.x + rhs.x, lhs.y + rhs.y}; }
  static inline Vec2 operator-(const Vec2 &lhs, const Vec2 &rhs) { return {lhs.x - rhs.x, lhs.y - rhs.y}; }
  static inline Vec2 operator*(const Vec2 &lhs, const Vec2 &rhs) { return {lhs.x * rhs.x, lhs.y * rhs.y}; }
  static inline Vec2 operator/(const Vec2 &lhs, const Vec2 &rhs) { return {lhs.x / rhs.x, lhs.y / rhs.y}; }

  static inline Vec2 operator*(const Vec2 &lhs, f32 rhs) { return {lhs.x * rhs, lhs.y * rhs}; }
  static inline Vec2 operator*(f32 lhs, const Vec2 &rhs) { return {lhs * rhs.x, lhs * rhs.y}; }
  static inline Vec2 operator/(const Vec2 &lhs, f32 rhs) { return {lhs.x / rhs, lhs.y / rhs}; }
  static inline Vec2 operator/(f32 lhs, const Vec2 &rhs) { return {lhs / rhs.x, lhs / rhs.y}; }
  static inline Vec2 operator+(const Vec2 &lhs, f32 rhs) { return {lhs.x + rhs, lhs.y + rhs}; }
  static inline Vec2 operator+(f32 lhs, const Vec2 &rhs) { return {lhs + rhs.x, lhs + rhs.y}; }
  static inline Vec2 operator-(const Vec2 &lhs, f32 rhs) { return {lhs.x - rhs, lhs.y - rhs}; }
  static inline Vec2 operator-(f32 lhs, const Vec2 &rhs) { return {lhs - rhs.x, lhs - rhs.y}; }

  static inline Vec2 &operator*=(Vec2 &lhs, f32 rhs)         { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
  static inline Vec2 &operator/=(Vec2 &lhs, f32 rhs)         { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
  static inline Vec2 &operator+=(Vec2 &lhs, f32 rhs)         { lhs.x += rhs; lhs.y += rhs; return lhs; }
  static inline Vec2 &operator-=(Vec2 &lhs, f32 rhs)         { lhs.x -= rhs; lhs.y -= rhs; return lhs; }
  static inline Vec2 &operator+=(Vec2 &lhs, const Vec2 &rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
  static inline Vec2 &operator-=(Vec2 &lhs, const Vec2 &rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
  static inline Vec2 &operator/=(Vec2 &lhs, const Vec2 &rhs) { lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs; }
  static inline Vec2 &operator*=(Vec2 &lhs, const Vec2 &rhs) { lhs.x /= rhs.x; lhs.y /= rhs.y; return lhs; }

  static inline bool operator==(const Vec2 &lhs, const Vec2 &rhs) { return (lhs.x == rhs.x) && (lhs.y == rhs.y); }
  static inline bool operator!=(const Vec2 &lhs, const Vec2 &rhs) { return !(lhs == rhs); }
}