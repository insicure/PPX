#pragma once

#include "Vec2.hpp"
#include "f32.hpp"

namespace ppx
{
  namespace math
  {
    inline f32 abs(const f32 &value)  { return (value < 0) ? -value : value; }
    inline int mod(const int num, const int den) { return mod32(num, den); }

    inline f32 cos(const int degrees) { return f32::Raw(cosLerp(degreesToAngle(degrees))); }
    inline f32 sin(const int degrees) { return f32::Raw(sinLerp(degreesToAngle(degrees))); }
    inline f32 tan(const int degrees) { return f32::Raw(tanLerp(degreesToAngle(degrees))); }

    inline f32 sqrt(const f32 &value) { return f32::Raw(sqrtf32(value.value)); }

    inline f32 distance(const Vec2 &p1, const Vec2 &p2)
    {
      f32 dx = p1.x - p2.x;
      f32 dy = p1.y - p2.y;
      return (dx*dx)+(dy*dy);
    }

    inline f32 distanceSqrt(const Vec2 &p1, const Vec2 &p2)
    {
      return sqrt(distance(p1, p2));
    }

    inline f32 distanceManhattan(const Vec2 &p1, const Vec2 &p2)
    {
      return abs(p1.x.toFloat() - p2.x.toFloat()) + abs(p1.y.toFloat() - p2.y.toFloat());
    }
  }
}