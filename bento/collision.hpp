#pragma once

#include "bento/struct.hpp"
namespace nb
{
  bool Check_PointCirlce(const Vec2 &point, const Vec2 &circle, const f32 &radius);
  bool Check_CircleCircle(const Vec2 &circle1, const f32 &radius1, const Vec2 &circle2, const f32 &radius2);
  bool Check_PointRect(const Vec2 &point, const Rect &rect);
  bool Check_RectRect(const Rect &rect1, const Rect &rect2);
  bool Check_CircleRect(const Vec2 &circle, const f32 &radius, const Rect &rect);
  bool Check_LinePoint(const Vec2 &start, const Vec2 &end, const Vec2 &point);
  bool Check_LineCircle(const Vec2 &start, const Vec2 &end, const Vec2 &circle, const f32 &radius);
  bool Check_LineLine(const Vec2 &start1, const Vec2 &end1, const Vec2 &start2, const Vec2 &end2);
  bool Check_LineRect(const Vec2 &start, const Vec2 &end, const Rect &rect);
  bool Check_PolyPoint(const Vec2 vertices[], const int length, const Vec2 &point);
  bool Check_PolyCircle(const Vec2 vertices[], const int length, const Vec2 &circle, const f32 &radius);
  bool Check_PolyRect(const Vec2 vertices[], const int length, const Rect &rect);
  bool Check_PolyLine(const Vec2 vertices[], const int length, const Vec2 &start, const Vec2 &end);
  bool Check_PolyLine(const Vec2 vertices1[], const int length1, const Vec2 vertices2[], const int length2);
}