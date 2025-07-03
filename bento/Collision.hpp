#pragma once

#include "Rect.hpp"
#include "Vec2.hpp"

namespace ppx
{
  struct Collider
  {
    static bool Point_Cirlce(const Vec2 point, const Vec2 circle, const f32 radius);
    static bool Circle_Circle(const Vec2 circle1, const f32 radius1, const Vec2 circle2, const f32 radius2);
    static bool Point_Rect(const Vec2 point, const Rect rect);
    static bool Rect_Rect(const Rect rect1, const Rect rect2);
    static bool Circle_Rect(const Vec2 circle, const f32 radius, const Rect rect);
    static bool Line_Point(const Vec2 start, const Vec2 end, const Vec2 point);
    static bool Line_Circle(const Vec2 start, const Vec2 end, const Vec2 circle, const f32 radius);
    static bool Line_Line(const Vec2 start1, const Vec2 end1, const Vec2 start2, const Vec2 end2);
    static bool Line_Rect(const Vec2 start, const Vec2 end, const Rect rect);
    static bool Poly_Point(const Vec2 vertices[], const int length, const Vec2 point);
    static bool Poly_Circle(const Vec2 vertices[], const int length, const Vec2 circle, const f32 radius);
    static bool Poly_Rect(const Vec2 vertices[], const int length, const Rect rect);
    static bool Poly_Line(const Vec2 vertices[], const int length, const Vec2 start, const Vec2 end);
    static bool Poly_Line(const Vec2 vertices1[], const int length1, const Vec2 vertices2[], const int length2);
  };
}