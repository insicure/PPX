#include "../TextureMap.hpp"
#include "../Math.hpp"
#include "../Rect.hpp"
#include "../Tracelog.hpp"
#include "../Vec2.hpp"
#include "nds/arm9/videoGL.h"

extern s32 _depth;

namespace ppx
{
  void TextureMap::Unload()
  {
    if (glDeleteTextures(1, &id) == 1)
    {
      TraceLog("Texture: unload id:%i", id);
      id = 0;
      width = 0;
      height = 0;
      hash = 0;
      frame_x = 0;
      frame_y = 0;
      offset_x = 0;
      offset_y = 0;
      frame_width = 0;
      frame_height = 0;
      rotated = false;
    }
    else TraceLog("Texture: failed unload id:%i", id);
  }

  bool TextureMap::isValid()
  {
    return (id > 0) && (width > 0) && (height > 0) && (hash != 0);
  }

  void TextureMap::Draw(const Vec2 &position, const int transform)
  {
    Draw(
      Rect(0, 0, frame_width, frame_height),
      Rect(position.x, position.y, frame_width, frame_height),
      transform,
      0,
      Vec2(0, 0)
    );
  }

  void TextureMap::Draw(const Vec2 &position, const int transform, const int rotation, const Vec2 &scale, const Vec2 origin)
  {
    Draw(
      Rect(0, 0, frame_width, frame_height),
      Rect(position.x, position.y, frame_width*scale.x, frame_height*scale.y),
      transform,
      rotation,
      origin
    );
  }

  void TextureMap::Draw(const Rect &region, const Rect &dest, const int transform, const int rotation, const Vec2 &origin)
  {
    // TODO: clamp region if bigger than frame
    Rect region_out = (rotated)
      ? Rect(
        (frame_width == region.width)
          ? frame_x + region.y
          : frame_x + frame_height - region.height - region.y,
        frame_y + region.x,
        region.height,
        region.width)
      : Rect(frame_x + region.x, frame_y + region.y, region.width, region.height);
    
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

    if (rotated)
    {
      tex_index[0] = (tex_index[0]+3) % 4;
      tex_index[1] = (tex_index[1]+3) % 4;
      tex_index[2] = (tex_index[2]+3) % 4;
      tex_index[3] = (tex_index[3]+3) % 4;
    }

    glBindTexture(GL_TEXTURE_2D, id);
    glBegin(GL_QUADS);
    {
      glPushMatrix();

      glTranslatef32(dest.x.toInt(), dest.y.toInt(), 0);
      if (rotation != 0) glRotateZi(degreesToAngle(rotation));
      glTranslatef32(-origin.x.toInt(), -origin.y.toInt(), 0);

      // top left
      glTexCoord2i(tex_coord[tex_index[0]].x.toInt(), tex_coord[tex_index[0]].y.toInt());
      glVertex3v16(0, 0, _depth);

      // bot left
      glTexCoord2i(tex_coord[tex_index[1]].x.toInt(), tex_coord[tex_index[1]].y.toInt());
      glVertex2v16(0, math::abs(dest.height).toInt());

      // bot right
      glTexCoord2i(tex_coord[tex_index[2]].x.toInt(), tex_coord[tex_index[2]].y.toInt());
      glVertex2v16(math::abs(dest.width).toInt(), math::abs(dest.height).toInt());

      // top right
      glTexCoord2i(tex_coord[tex_index[3]].x.toInt(), tex_coord[tex_index[3]].y.toInt());
      glVertex2v16(math::abs(dest.width).toInt(), 0);

      glPopMatrix(1);
    }
    glEnd();
    _depth++;
  }

}