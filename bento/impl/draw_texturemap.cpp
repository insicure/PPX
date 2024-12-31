#include "bento/drawing.hpp"
#include "bento/struct.hpp"
#include "nds/arm9/videoGL.h"
#include <cstring>

extern s32 _depth;

namespace nb
{
  void DrawTextureMap(const TextureAtlas &atlas, const char* name, const Vec2 &position, const int rotation, const f32 &scale)
  {
    TextureMap *texture = atlas.Find(name);
    if (texture)
    {
      Rect source = (texture->rotated)
        ? Rect(texture->x, texture->y, texture->frame_height, texture->frame_width)
        : Rect(texture->x, texture->y, texture->frame_width, texture->frame_height);

      Rect dest = (texture->rotated)
        ? Rect(position.x, position.y, texture->frame_height*scale, texture->frame_width*scale)
        : Rect(position.x, position.y, texture->frame_width*scale, texture->frame_height*scale);

      Vec2 origin(texture->frame_x, texture->frame_y);

      glBindTexture(GL_TEXTURE_2D, texture->id);
      glBegin(GL_QUADS);
      {
        glPushMatrix();
        glTranslatef32(dest.x.Geti(), dest.y.Geti(), 0);
        if (rotation != 0) glRotateZi(degreesToAngle(rotation));
        glTranslatef32((-origin.x).Geti(), (-origin.y).Geti(), 0);

        if (!texture->rotated)
        {
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
        }
        else
        {
          glTexCoord2i(source.Right().Geti(), source.Top().Geti());
          glVertex3v16(0, 0, _depth);

          glTexCoord2i(source.Left().Geti(), source.Top().Geti());
          glVertex2v16(0, dest.width.Geti());

          glTexCoord2i(source.Left().Geti(), source.Bottom().Geti());
          glVertex2v16(dest.height.Geti(), dest.width.Geti());

          glTexCoord2i(source.Right().Geti(), source.Bottom().Geti());
          glVertex2v16(dest.height.Geti(), 0);
        }

        glPopMatrix(1);
      }
      glEnd();

      _depth++;

      // Vec2 offset = (texture->rotated)
      // ? Vec2(
      //   texture->frame_y+texture->frame_x - texture->height*texture->pivotY,
      //   -texture->offsetX + texture->width*texture->pivotX)
      // : Vec2(
      //   -texture->offsetX + texture->width*texture->pivotX,
      //   -texture->frame_y + texture->height*texture->pivotY);

      // offset.x = offset.x * scale;
      // offset.y = offset.y * scale;

      // DrawTexture(
      //   texture,
      //   {texture->x, texture->y, texture->frame_x, texture->frame_y},
      //   {position.x, position.y, texture->frame_x*scale, texture->frame_y*scale},
      //   {0, 0},
      //   ((texture->rotated) ? -90 : 0) + rotation
      // );
    }
  }
}