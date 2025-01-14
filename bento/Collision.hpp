#pragma once

#include "Rect.hpp"
#include "Vec2.hpp"

namespace ppx
{
  namespace collision
  {
    bool Point_Cirlce(const Vec2 &point, const Vec2 &circle, const f32 &radius);
    bool Circle_Circle(const Vec2 &circle1, const f32 &radius1, const Vec2 &circle2, const f32 &radius2);
    bool Point_Rect(const Vec2 &point, const Rect &rect);
    bool Rect_Rect(const Rect &rect1, const Rect &rect2);
    bool Circle_Rect(const Vec2 &circle, const f32 &radius, const Rect &rect);
    bool Line_Point(const Vec2 &start, const Vec2 &end, const Vec2 &point);
    bool Line_Circle(const Vec2 &start, const Vec2 &end, const Vec2 &circle, const f32 &radius);
    bool Line_Line(const Vec2 &start1, const Vec2 &end1, const Vec2 &start2, const Vec2 &end2);
    bool Line_Rect(const Vec2 &start, const Vec2 &end, const Rect &rect);
    bool Poly_Point(const Vec2 vertices[], const int length, const Vec2 &point);
    bool Poly_Circle(const Vec2 vertices[], const int length, const Vec2 &circle, const f32 &radius);
    bool Poly_Rect(const Vec2 vertices[], const int length, const Rect &rect);
    bool Poly_Line(const Vec2 vertices[], const int length, const Vec2 &start, const Vec2 &end);
    bool Poly_Line(const Vec2 vertices1[], const int length1, const Vec2 vertices2[], const int length2);
  }
}