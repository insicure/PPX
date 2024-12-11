#include "bento/color.hpp"
#include "bento/struct.hpp"
#include "gl2d.h"

s32 _depth = 0;

namespace nb
{
  void Initialize()
  {
    glScreen2D();
  }

  void ClearBackground(const Color &color)
  {
    glClearColor(
      (color.r >> 3) & 0x1F,
      (color.g >> 3) & 0x1F,
      (color.b >> 3) & 0x1F,
      (color.a >> 15) & 0x01
    );
  }

  void BeginDrawing()
  {
    glBegin2D();
    _depth = 0;
  }

  void EndDrawing()
  {
    glEnd2D();
    glFlush(0);
  }

  void BeginCamera(const Camera &cam)
  {
    glPushMatrix();
    glTranslatef32(-cam.target.x.value, -cam.target.y.value, 0);
    glRotateZi(degreesToAngle(cam.rotation));
    glScalef32(cam.zoom.value, cam.zoom.value, f32(1).value);
    glTranslatef32(cam.offset.x.value, cam.offset.y.value, 0);
  }

  void EndCamera()
  {
    glPopMatrix(1);
  }

  void SetColor(const Color &color)
  {
    glColor(RGB8(color.r, color.g, color.b));
  }

  void SetAlpha(const f32 &ratio)
  {
    glPolyFmt(POLY_ALPHA((31*ratio).Geti()) | POLY_CULL_BACK);
  }

  void SetAlphai(const int value)
  {
    glPolyFmt(POLY_ALPHA(value) | POLY_CULL_BACK);
  }
}