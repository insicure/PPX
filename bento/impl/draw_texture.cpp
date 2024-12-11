
#include "drawing.hpp"
#include "nds/arm9/videoGL.h"

extern s32 _depth;

namespace nb
{
  void DrawTexture(const Texture &texture, const Vec2 &position)
  {
    DrawTexture(texture, position, 0, 1);
  }

  void DrawTexture(const Texture &texture, const Vec2 &position, const int rotation, const f32 &scale)
  {
    Rect source(0, 0, texture.width, texture.height);
    Rect dest(position.x, position.y, texture.width*scale, texture.height*scale);
    Vec2 origin(0, 0);

    DrawTexture(texture, source, dest, origin, rotation);
  }

  void DrawTexture(const Texture &texture, const Rect &source, const Vec2 &position)
  {
    Rect dest(position.x, position.y, source.width, source.height);
    Vec2 origin(0, 0);

    DrawTexture(texture, source, dest, origin, 0);
  }

  void DrawTexture(const Texture &texture, const Rect &source, const Rect &dest, const Vec2 &origin, const int rotation)
  {
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glBegin(GL_QUADS);
    {
      glPushMatrix();
      glTranslatef32(dest.x.Geti(), dest.y.Geti(), 0);
      if (rotation != 0) glRotateZi(degreesToAngle(rotation));
      glTranslatef32((-origin.x).Geti(), (-origin.y).Geti(), 0);

      // top left
      glTexCoord2i(source.Left().Geti(), source.Top().Geti());
      glVertex3v16(0, 0, _depth);

      // bottom left
      glTexCoord2i(source.Left().Geti(), source.Bottom().Geti());
      glVertex2v16(0, dest.height.Geti());

      // bottom right
      glTexCoord2i(source.Right().Geti(), source.Bottom().Geti());
      glVertex2v16(dest.width.Geti(), dest.height.Geti());

      // top right
      glTexCoord2i(source.Right().Geti(), source.Top().Geti());
      glVertex2v16(dest.width.Geti(), 0);

      glPopMatrix(1);
    }
    glEnd();

    _depth++;
  }


  void DrawTextureTiled(const Texture &texture, const Rect &source, const Rect &dest, const Vec2 &origin, const int rotation, const f32 &scale)
  {
    sassert(false, "currently not supported!");
  }
}