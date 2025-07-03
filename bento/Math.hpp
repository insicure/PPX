#pragma once

#include "Vec2.hpp"
#include "f32.hpp"

namespace ppx
{
  struct Math
  {
    static inline f32 abs(f32 value)  { return (value < 0) ? -value : value; }
    static inline int mod(const int num, const int den) { return mod32(num, den); }

    static inline f32 cos(int degrees) { return f32::Raw(cosLerp(degreesToAngle(degrees))); }
    static inline f32 sin(int degrees) { return f32::Raw(sinLerp(degreesToAngle(degrees))); }
    static inline f32 tan(int degrees) { return f32::Raw(tanLerp(degreesToAngle(degrees))); }

    static inline f32 sqrt(f32 value) { return f32::Raw(sqrtf32(value.value)); }

    static inline f32 distance(const Vec2 p1, const Vec2 p2)
    {
      f32 dx = p1.x - p2.x;
      f32 dy = p1.y - p2.y;
      return (dx*dx)+(dy*dy);
    }

    static inline f32 distanceSqrt(const Vec2 p1, const Vec2 p2)
    {
      return sqrt(distance(p1, p2));
    }

    static inline f32 distanceManhattan(const Vec2 p1, const Vec2 p2)
    {
      return abs(p1.x.toFloat() - p2.x.toFloat()) + abs(p1.y.toFloat() - p2.y.toFloat());
    }
  };
}