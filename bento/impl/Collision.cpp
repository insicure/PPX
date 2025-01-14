#include "../Collision.hpp"
#include "../Math.hpp"

namespace ppx
{

  bool collision::Point_Cirlce(const Vec2 &point, const Vec2 &circle, const f32 &radius)
  {
    f32 distance = math::distance(point, circle);
    if (distance <= radius*radius) return true;
    return false;
  }

  bool collision::Circle_Circle(const Vec2 &circle1, const f32 &radius1, const Vec2 &circle2, const f32 &radius2)
  {
    f32 distance = math::distance(circle1, circle2);
    if (distance <= (radius1+radius2)*(radius1+radius2)) return true;
    return false;
  }

  bool collision::Point_Rect(const Vec2 &point, const Rect &rect)
  {
    if (point.x >= rect.Left() &&
        point.x <= rect.Right() &&
        point.y >= rect.Top() &&
        point.y <= rect.Bottom())
    {
      return true;
    }

    return false;
  }

  bool collision::Rect_Rect(const Rect &rect1, const Rect &rect2)
  {
    if (rect1.Right()  >= rect2.Left()  &&
        rect1.Left()   <= rect2.Right() &&
        rect1.Bottom() >= rect2.Top()   &&
        rect1.Top()    <= rect2.Bottom())
    {
      return true;
    }

    return false;
  }

  bool collision::Circle_Rect(const Vec2 &circle, const f32 &radius, const Rect &rect)
  {
    // temporary variables to set edges for testing
    Vec2 test = circle;

    // which edge is closest?
    if (circle.x < rect.Left())        test.x = rect.Left();    // test left edge
    else if (circle.x > rect.Right())  test.x = rect.Right();   // right edge
    if (circle.y < rect.Top())         test.y = rect.Top();     // top edge
    else if (circle.y > rect.Bottom()) test.y = rect.Bottom();  // bottom edge

    f32 distance = math::distance(circle, test);
    if (distance <= radius*radius) return true;
    return false;
  }

  bool collision::Line_Point(const Vec2 &start, const Vec2 &end, const Vec2 &point)
  {
    f32 d1 = math::distanceSqrt(point, start);
    f32 d2 = math::distanceSqrt(point, end);

    f32 lineLen = math::distanceSqrt(start, end);
    f32 buffer = 0.1f;

    if (d1+d2 >= lineLen-buffer && d1+d2 <= lineLen+buffer)
      return true;

    return false;
  }

  bool collision::Line_Circle(const Vec2 &start, const Vec2 &end, const Vec2 &circle, const f32 &radius)
  {
    // is either end INSIDE the circle?
    // if so, return true immediately
    bool inside1 = collision::Point_Cirlce(start, circle, radius);
    bool inside2 = collision::Point_Cirlce(end, circle, radius);
    if (inside1 || inside2) return true;

    // get length of the line
    f32 len = math::distance(start, end);

    // get dot product of the line and circle
    f32 dot = ( ((circle.x-start.x)*(end.x-start.x)) + ((circle.y-start.y)*(end.y-start.y)) ) / (len);

    // find the closest point on the line
    Vec2 closest(
      start.x + (dot * (end.x-start.x)),
      start.y + (dot * (end.y-start.y))
    );

    // is this point actually on the line segment?
    // if so keep going, but if not, return false
    bool onSegment = collision::Line_Point(start, end, closest);
    if (!onSegment) return false;

    // optionally, draw a circle at the closest
    // point on the line
    // DrawCircle(closest, 5);

    // get distance to closest point
    f32 distance = math::distance(closest, circle);
    if (distance <= radius*radius) return true;
    return false;
  }

  bool collision::Line_Line(const Vec2 &start1, const Vec2 &end1, const Vec2 &start2, const Vec2 &end2)
  {
    // calculate the distance to intersection point
    f32 uA = ((end2.x-start2.x)*(start1.y-start2.y) - (end2.y-start2.y)*(start1.x-start2.x)).toFloat() / ((end2.y-start2.y)*(end1.x-start1.x) - (end2.x-start2.x)*(end1.y-start1.y)).toFloat();
    f32 uB = ((end1.x-start1.x)*(start1.y-start2.y) - (end1.y-start1.y)*(start1.x-start2.x)).toFloat() / ((end2.y-start2.y)*(end1.x-start1.x) - (end2.x-start2.x)*(end1.y-start1.y)).toFloat();

    // if uA and uB are between 0-1, lines are colliding
    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {

      // optionally, draw a circle where the lines meet
      // Vec2 intersection(
      //   start1.x + (uA * (end1.x-start1.x)),
      //   start1.y + (uA * (end1.y-start1.y)));
      // DrawCircle(intersection, 5);

      return true;
    }
    return false;
  }

  bool collision::Line_Rect(const Vec2 &start, const Vec2 &end, const Rect &rect)
  {
    // check if the line has hit any of the rectangle's sides
    // uses the Line/Line function below
    bool left =   collision::Line_Line(start, end, {rect.Left(), rect.Top()}, {rect.Left(), rect.Bottom()});
    bool right =  collision::Line_Line(start, end, {rect.Right(), rect.Top()}, {rect.Right(), rect.Bottom()});
    bool top =    collision::Line_Line(start, end, {rect.x, rect.y}, {rect.x+rect.width, rect.y});
    bool bottom = collision::Line_Line(start, end, {rect.x, rect.y+rect.height}, {rect.x+rect.width, rect.y+rect.height});

    // if ANY of the above are true, the line
    // has hit the rectangle
    if (left || right || top || bottom) 
      return true;

    return false;
  }

  bool collision::Poly_Point(const Vec2 vertices[], const int length, const Vec2 &point)
  {
    bool collision = false;

    int next = 0;
    for (int current=0; current<length; current++) {
      next = (current+1) % length;

      const Vec2 &vc = vertices[current];
      const Vec2 &vn = vertices[next];

      if (((vc.y >= point.y && vn.y < point.y) || (vc.y < point.y && vn.y >= point.y)) &&
           (point.x < (vn.x-vc.x)*(point.y-vc.y) / (vn.y-vc.y)+vc.x)) {
              collision = !collision;
      }
    }

    return collision;
  }

  bool collision::Poly_Circle(const Vec2 vertices[], const int length, const Vec2 &circle, const f32 &radius)
  {
    int next = 0;
    for (int current=0; current<length; current++) {
      next = (current+1) % length;

      const Vec2 &vc = vertices[current];
      const Vec2 &vn = vertices[next];

      bool collision = collision::Line_Circle(vc, vn, circle, radius);
      if (collision) return true;
    }

    // Optional
    // bool centerInside = Check_PolyPoint(vertices, length, circle);
    // if (centerInside) return true;

    return false;
  }
  
  bool collision::Poly_Rect(const Vec2 vertices[], const int length, const Rect &rect)
  {
    int next = 0;
    for (int current=0; current<length; current++) {
      next = (current+1) % length;

      const Vec2 &vc = vertices[current];
      const Vec2 &vn = vertices[next];

      bool collision = collision::Line_Rect(vc, vn, rect);
      if (collision) 
        return true;

      // optional: test if the rectangle is INSIDE the polygon
      // note that this iterates all sides of the polygon
      // again, so only use this if you need to
      // bool inside = Check_PolyPoint(vertices, length, {rect.x, rect.y});
      // if (inside) return true;

    }

    return false;
  }
  
  bool collision::Poly_Line(const Vec2 vertices[], const int length, const Vec2 &start, const Vec2 &end)
  {
    int next = 0;
    for (int current=0; current<length; current++) {
      next = (current+1) % length;

      const Vec2 &vc = vertices[current];
      const Vec2 &vn = vertices[next];

      bool collision = collision::Line_Line(vc, vn, start, end);
      if (collision) return true;
    }

    return false;
  }
  
  bool collision::Poly_Line(const Vec2 vertices1[], const int length1, const Vec2 vertices2[], const int length2)
  {
    int next = 0;
    for (int current=0; current<length1; current++)
    {
      next = (current+1) % length1;

      const Vec2 &vc = vertices1[current];
      const Vec2 &vn = vertices1[next];

      bool collision = collision::Poly_Line(vertices2, length2, vc, vn);
      if (collision) return true;

      // optional: check if the 2nd polygon is INSIDE the first
      // collision = Check_PolyPoint(vertices1, length1, {vertices2[0].x, vertices2[0].y});
      // if (collision) return true;
    }

    return false;
  }
  
}