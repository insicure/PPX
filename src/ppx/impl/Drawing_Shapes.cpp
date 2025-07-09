#include "../Drawing.hpp"
#include "../Math.hpp"
#include <cstdint>
#include <gl2d.h>
#include <nds/arm9/video.h>
#include <nds/arm9/videoGL.h>

extern s32 _depth;

namespace ppx
{

  void DrawPixel(const Vec2 &position)
  {
    glBindTexture(0, 0);
    glBegin(GL_TRIANGLES);
    {
      glVertex3v16(position.x.toInt(), position.y.toInt(), _depth);
      glVertex2v16(position.x.toInt(), (position.y+1).toInt());
      glVertex2v16((position.x+1).toInt(), position.y.toInt());
    }
    glEnd();
    _depth++;
  }


  void DrawLine(const Vec2 &startPos, const Vec2 &endPos)
  {
    glBindTexture(0, 0);
    glBegin(GL_TRIANGLES);
    {
      glVertex3v16(startPos.x.toInt(), startPos.y.toInt(), _depth);
      glVertex2v16(endPos.x.toInt(), endPos.y.toInt());
      glVertex2v16(endPos.x.toInt(), endPos.y.toInt());
    }
    glEnd();
    _depth++;
  }

  void DrawLine(const Vec2 &startPos, const Vec2 &endPos, const f32 &thick)
  {
    const Vec2 delta(endPos.x-startPos.x, endPos.y-startPos.y);

    const f32 length = f32::Raw(sqrtf32((delta.x*delta.x).value + (delta.y*delta.y).value));

    if ((length.value > 0) && (thick.value > 0))
    {
      const f32 scale(thick / (length*2));
      const Vec2 radius(-scale*delta.y, scale*delta.x);

      glBindTexture(0, 0);
      glBegin(GL_TRIANGLE_STRIP);
        glVertex3v16((startPos.x-radius.x).toInt(), (startPos.y-radius.y).toInt(), _depth);
        glVertex2v16((startPos.x+radius.x).toInt(), (startPos.y+radius.y).toInt());
        glVertex2v16((endPos.x-radius.x).toInt(), (endPos.y-radius.y).toInt());
        glVertex2v16((endPos.x+radius.x).toInt(), (endPos.y+radius.y).toInt());
      glEnd();
      _depth++;
    }
  }


  void DrawRect(const Rect &rec)
  {
    DrawRect(rec, Vec2(0, 0), 0);
  }

  void DrawRect(const Rect &rec, const Vec2 &origin, const int16_t rotation)
  {
    glBindTexture(0, 0);
    glBegin(GL_TRIANGLE_STRIP);
    {
      glPushMatrix();

      glTranslatef32(rec.Left().toInt(), rec.Top().toInt(), 0);
      if (rotation != 0) glRotateZi(degreesToAngle(rotation));
      glTranslatef32((-origin.x).toInt(), (-origin.y).toInt(), 0);

      glVertex3v16(0, 0, _depth);
      glVertex2v16(0, rec.height.toInt());
      glVertex2v16(rec.width.toInt(), 0);
      glVertex2v16(rec.width.toInt(), rec.height.toInt());

      glPopMatrix(1);
    }
    glEnd();
    _depth++;
  }

  void DrawRectLines(const Rect &rec)
  {
    glBindTexture(0, 0);
    glBegin( GL_TRIANGLES );
      glVertex3v16(rec.Left().toInt(), rec.Top().toInt(), _depth);
      glVertex2v16(rec.Right().toInt(), rec.Top().toInt());
      glVertex2v16(rec.Right().toInt(), rec.Top().toInt());

      glVertex3v16((rec.Right()-1).toInt(), rec.Top().toInt(), _depth);
      glVertex2v16(rec.Right().toInt(), rec.Bottom().toInt());
      glVertex2v16(rec.Right().toInt(), rec.Bottom().toInt());

      glVertex3v16(rec.Right().toInt(), rec.Bottom().toInt(), _depth);
      glVertex2v16(rec.Left().toInt(), rec.Bottom().toInt());
      glVertex2v16(rec.Left().toInt(), rec.Bottom().toInt());

      glVertex3v16((rec.Left()+1).toInt(), rec.Bottom().toInt(), _depth);
      glVertex2v16(rec.Left().toInt(), rec.Top().toInt());
      glVertex2v16(rec.Left().toInt(), rec.Top().toInt());
    glEnd();
    _depth++;
  }

  void DrawRectLines(const Rect &rec, const f32 &lineThick)
  {
    f32 thick;

    if ((lineThick > rec.width) || (lineThick > rec.height))
    {
      if (rec.width > rec.height) thick = rec.height/2;
      else if (rec.width < rec.height) thick = rec.width/2;
    }

    // When rec = { x, y, 8.0f, 6.0f } and lineThick = 2, the following
    // four rectangles are drawn ([T]op, [B]ottom, [L]eft, [R]ight):
    //
    //   TTTTTTTT
    //   TTTTTTTT
    //   LL    RR
    //   LL    RR
    //   BBBBBBBB
    //   BBBBBBBB
    //

    Rect top(rec.x, rec.y, rec.width, lineThick);
    Rect bottom(rec.x, rec.y - lineThick + rec.height, rec.width, lineThick);
    Rect left(rec.x, rec.y + lineThick, lineThick, rec.height - lineThick*2 + 1);
    Rect right(rec.x - lineThick + rec.width + 1, rec.y + lineThick, lineThick, rec.height - lineThick*2 + 1);

    DrawRect(top);
    DrawRect(bottom);
    DrawRect(left);
    DrawRect(right);
    _depth++;
  }


  void DrawTriangle(const Vec2 &v1, const Vec2 &v2, const Vec2 &v3)
  {
    glBindTexture(0, 0);
    glBegin(GL_TRIANGLES);
      glVertex3v16(v1.x.toInt(), v1.y.toInt(), _depth);
      glVertex2v16(v2.x.toInt(), v2.y.toInt());
      glVertex2v16(v3.x.toInt(), v3.y.toInt());
    glEnd();
    _depth++;
  }

  void DrawTriangleLines(const Vec2 &v1, const Vec2 &v2, const Vec2 &v3)
  {
    glBindTexture(0, 0);
    glBegin( GL_TRIANGLES );
      glVertex3v16(v1.x.toInt(), v1.y.toInt(), _depth);
      glVertex2v16(v2.x.toInt(), v2.y.toInt());
      glVertex2v16(v2.x.toInt(), v2.y.toInt());

      glVertex3v16(v2.x.toInt(), v2.y.toInt(), _depth);
      glVertex2v16(v3.x.toInt(), v3.y.toInt());
      glVertex2v16(v3.x.toInt(), v3.y.toInt());

      glVertex3v16(v3.x.toInt(), v3.y.toInt(), _depth);
      glVertex2v16(v1.x.toInt(), v1.y.toInt());
      glVertex2v16(v1.x.toInt(), v1.y.toInt());
    glEnd();
    _depth++;
  }


  void DrawCircle(const Vec2 &position, f32 radius)
  {
    if (radius <= 0) radius = 0.1f;

    int startAngle = 0;
    int endAngle = 360;
    int segments = 18;

    int step = (endAngle - startAngle) / segments;
    int angle = startAngle;

    glBindTexture(0, 0);
    glBegin(GL_QUAD);

      for (int i=0; i<segments/2; i++)
      {
        glVertex3v16(
          position.x.toInt(),
          position.y.toInt(),
          _depth);

        glVertex2v16(
          (position.x + Math::cos(angle+step*2)*radius).toInt(),
          (position.y + Math::sin(angle+step*2)*radius).toInt());

        glVertex2v16(
          (position.x + Math::cos(angle+step)*radius).toInt(),
          (position.y + Math::sin(angle+step)*radius).toInt());

        glVertex2v16(
          (position.x + Math::cos(angle)*radius).toInt(),
          (position.y + Math::sin(angle)*radius).toInt());

        angle += (step*2);
      }
    glEnd();
    _depth++;
  }

  void DrawCircleLines(const Vec2 &position, f32 radius)
  {
    glBindTexture(0, 0);
    glBegin(GL_TRIANGLES);
    {
      int segments = 20;
      for (int i=0; i<360; i+=segments)
      {
        glVertex3v16(
          (position.x + Math::cos(i)*radius).toInt(),
          (position.y + Math::sin(i)*radius).toInt(),
          _depth);

        glVertex2v16(
          (position.x + Math::cos(i+segments)*radius).toInt(),
          (position.y + Math::sin(i+segments)*radius).toInt());

        glVertex2v16(
          (position.x + Math::cos(i+segments)*radius).toInt(),
          (position.y + Math::sin(i+segments)*radius).toInt());
      }
    }
    glEnd();
    _depth++;
  }


  void DrawPolyLines(const Vec2 vertices[], const int16_t length)
  {
    for (int i=0; i<length; i++)
    {
      const Vec2 &vc = vertices[i];
      const Vec2 &vn = vertices[(i+1) % length];

      DrawLine(vc, vn);
    }
    _depth++;
  }

  void DrawPolyLines(const Vec2 vertices[], const int16_t length, const f32 &thick)
  {
    for (int i=0; i<length; i++)
    {
      const Vec2 &vc = vertices[i];
      const Vec2 &vn = vertices[(i+1) % length];

      DrawLine(vc, vn, thick);
    }
    _depth++;
  }

}