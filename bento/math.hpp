#pragma once

#include "bento/f32.hpp"
#include "bento/struct.hpp"

#include <cstdlib>
#include <nds/arm9/math.h>
#include <nds/arm9/trig_lut.h>
namespace nb
{
  inline f32 Cosf32(const int degrees) { return f32::Raw(cosLerp(degreesToAngle(degrees))); }
  inline f32 Sinf32(const int degrees) { return f32::Raw(sinLerp(degreesToAngle(degrees))); }
  inline f32 Tanf32(const int degrees) { return f32::Raw(tanLerp(degreesToAngle(degrees))); }

  inline f32 Sqrtf32(const f32 &value) { return f32::Raw(sqrtf32(value.Geti())); }

  inline f32 Distance(const Vec2 &p1, const Vec2 &p2)
  {
    f32 dx = p1.x - p2.x;
    f32 dy = p1.y - p2.y;
    return (dx*dx)+(dy*dy);
  }

  inline f32 DistanceSqrt(const Vec2 &p1, const Vec2 &p2)
  {
    return Sqrtf32(Distance(p1, p2));
  }

  inline f32 distanceManhattan(const Vec2 &p1, const Vec2 &p2)
  {
    return std::abs(p1.x.Getf() - p2.x.Getf()) + std::abs(p1.y.Getf() - p2.y.Getf());
  }

}