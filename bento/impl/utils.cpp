#include "bento/utils.hpp"
#include "bento/math.hpp"
#include "nds/arm9/input.h"
#include <cstdarg>
#include <cstdio>

static touchPosition touchtemp;

namespace nb {

  void TraceLog(const char *format, ...)
  {
    #ifndef DISABLE_TRACELOG
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    // simply append a newline after printing
    fprintf(stderr, "\n");
    #endif
  }

  Vec2 GetCameraToScreen(const Vec2 &position, const Camera &camera)
  {
    Matrix translate(
      1, 0, position.x,
      0, 1, position.y,
      0, 0, 1);

    Matrix m = translate * GetCameraMatrix(camera);
    return Vec2(m.m02, m.m12);
  }

  Vec2 GetScreenToCamera(const Vec2 &position, const Camera &camera)
  {
    Matrix translate(
      1, 0, position.x,
      0, 1, position.y,
      0, 0, 1);

    Matrix m = translate * Matrix::Inverse(GetCameraMatrix(camera));
    return Vec2(m.m02, m.m12);
  }

  Matrix GetCameraMatrix(const Camera &camera)
  {
    Matrix origin(
      1, 0, -camera.target.x,
      0, 1, -camera.target.y,
      0, 0, 1);

    Matrix rotation(
      Cosf32(camera.rotation), -Sinf32(camera.rotation), 0,
      Sinf32(camera.rotation), Cosf32(camera.rotation), 0,
      0, 0, 1);

    Matrix scale(
      camera.zoom, 0, 0,
      0, camera.zoom, 0,
      0, 0, 1);

    Matrix translate(
      1, 0, camera.offset.x,
      0, 1, camera.offset.y,
      0, 0, 1);

    return origin * (scale * rotation) * translate;
  }

  bool GetTouch(TouchPhase phase, touchPosition &touch) {
    touchRead(&touch);

    switch (phase){
      case TouchPhase_RELEASE:
      {
        // store prev pos because px,py return 0 when KeysUp()
        if (keysHeld() & KEY_TOUCH) touchtemp = touch;
        if (keysUp() & KEY_TOUCH)
        {
          touch = touchtemp;
          return true;
        }
        return false;
      }

      case TouchPhase_DOWN: return (keysDown() & KEY_TOUCH);
      case TouchPhase_HELD: return (keysDown() & KEY_TOUCH);
    }

    return false;
  }
}