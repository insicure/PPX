
#include "bento/drawing.hpp"
#include "bento/struct.hpp"
#include "bento/utils.hpp"
#include "nds/arm9/trig_lut.h"
#include "nds/arm9/videoGL.h"

extern s32 _depth;

namespace nb
{
  void DrawTexture(const Texture &texture, const int transform, const Vec2 &position)
  {
    TextureFrame frame = texture.GetFrame();

    DrawTexture(
      texture,
      {0, 0, frame.width, frame.height},
      transform,
      {position.x, position.y, frame.width, frame.height},
      0,
      {0, 0});
  }

  void DrawTexture(const Texture &texture, const int transform, const Vec2 &position, const int rotation, const Vec2 &scale, const Vec2 origin)
  {
    TextureFrame frame = texture.GetFrame();

    DrawTexture(
      texture,
      {0, 0, frame.width, frame.height},
      transform,
      {position.x, position.y, frame.width*scale.x, frame.height*scale.y},
      rotation,
      origin);
  }

  void DrawTexture(const Texture &texture, const Rect &region, const int transform, const Rect &dest, const int rotation, const Vec2 &origin)
  {
    /* 

      A     W     D
       -----------
      |           |
      |           | H
      |           |
       -----------
      B           C

      TL BL BR TR    1,  1   ABCD 0123
      TR BR BL TL   -1,  1   DCBA 3210
      BL TL TR BR    1, -1   BADC 1032
      BR TR TL BL   -1, -1   CDAB 2301
    */

    TextureFrame frame = texture.GetFrame();
    // TraceLog("%i %i %i %i", frame.x, frame.y, frame.width, frame.height, frame.rotated);
    
    const Rect region_out = (frame.rotated)
      ? Rect(frame.x + region.y, frame.y + region.x, region.height, region.width)
      : Rect(frame.x + region.x, frame.y + region.y, region.width, region.height);
    
    const Rect dest_out = (frame.rotated)
      ? Rect(dest.x, dest.y, dest.height, dest.width)
      : Rect(dest.x, dest.y, dest.width, dest.height);


    Vec2 tex_coord[4] = {
      {region_out.Left(), region_out.Top()},
      {region_out.Left(), region_out.Bottom()},
      {region_out.Right(), region_out.Bottom()},
      {region_out.Right(), region_out.Top()}
    };

    int tex_index[4];
    switch (transform)
    {
      default:
      case TRANS_NONE:
      tex_index[0] = 0;
      tex_index[1] = 1;
      tex_index[2] = 2;
      tex_index[3] = 3;
      break;

      case TRANS_FLIP_X:
      tex_index[0] = 3;
      tex_index[1] = 2;
      tex_index[2] = 1;
      tex_index[3] = 0;
      break;

      case TRANS_FLIP_Y:
      tex_index[0] = 1;
      tex_index[1] = 0;
      tex_index[2] = 3;
      tex_index[3] = 2;
      break;

      case TRANS_FLIP_X|TRANS_FLIP_Y:
      tex_index[0] = 2;
      tex_index[1] = 3;
      tex_index[2] = 0;
      tex_index[3] = 1;
      break;
    }

    if (frame.rotated)
    {
      tex_index[0] = (tex_index[0]+3) % 4;
      tex_index[1] = (tex_index[1]+3) % 4;
      tex_index[2] = (tex_index[2]+3) % 4;
      tex_index[3] = (tex_index[3]+3) % 4;
    }

    glBindTexture(GL_TEXTURE_2D, texture.id);
    glBegin(GL_QUADS);
    {
      glPushMatrix();

      glTranslatef32(dest_out.x.Geti(), dest_out.y.Geti(), 0);
      if (rotation != 0) glRotateZi(degreesToAngle(rotation));
      glTranslatef32(-origin.x.Geti(), -origin.y.Geti(), 0);

      // top left
      glTexCoord2i(tex_coord[tex_index[0]].x.Geti(), tex_coord[tex_index[0]].y.Geti());
      glVertex3v16(0, 0, _depth);

      // bot left
      glTexCoord2i(tex_coord[tex_index[1]].x.Geti(), tex_coord[tex_index[1]].y.Geti());
      glVertex2v16(0, dest_out.height.Geti());

      // bot right
      glTexCoord2i(tex_coord[tex_index[2]].x.Geti(), tex_coord[tex_index[2]].y.Geti());
      glVertex2v16(dest_out.width.Geti(), dest_out.height.Geti());

      // top right
      glTexCoord2i(tex_coord[tex_index[3]].x.Geti(), tex_coord[tex_index[3]].y.Geti());
      glVertex2v16(dest_out.width.Geti(), 0);

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