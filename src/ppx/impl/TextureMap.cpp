#include "../TextureMap.hpp"
#include "nds/arm9/videoGL.h"
#include <utility>

extern s32 _depth;

namespace ppx
{
  void TextureMap::Draw(const Vec2 &position, const Vec2 &scale,const Vec2 &origin,int rotation,bool flip_x, bool flip_y,const Rect &region, const Color tint)
  {
    // TODO: region crop might not working, not thoroughly tested!
    // Default to full texture frame size if not specified
    const int region_width = ((region.width > 0) ? region.width.toInt() : frame_width);
    const int region_height = ((region.height > 0) ? region.height.toInt() : frame_height);
    
    // Base texture coordinates
    const int base_x = frame_x + region.x.toInt();
    const int base_y = frame_y + region.y.toInt();

    const int x1 = base_x;
    const int y1 = base_y;
    const int x2 = base_x + region_width;
    const int y2 = base_y + region_height;

    // Handle flipping
    int texcoords[4][2] = {
      {flip_x ? x2 : x1, flip_y ? y2 : y1},   // top-left flipped
      {flip_x ? x1 : x2, flip_y ? y2 : y1},   // top-right flipped
      {flip_x ? x1 : x2, flip_y ? y1 : y2},   // bottom-right flipped
      {flip_x ? x2 : x1, flip_y ? y1 : y2}    // bottom-left flipped
    };

    // Handle rotated
    if (rotated) {
      for (int i=0; i<4; i++) {
        std::swap(texcoords[i][0], texcoords[i][1]);
        texcoords[i][1] = frame_y + region_height - texcoords[i][1];
      }
    }

    glPushMatrix();
    glTranslatef32(position.x.toInt(), position.y.toInt(), 0);
    glTranslatef32(offset_x, offset_y, 0);
    if (rotation != 0) glRotateZi(degreesToAngle(rotation));
    glTranslatef32(-origin.x.toInt(), -origin.y.toInt(), 0);
    glScalef32(scale.x.value, scale.y.value, f32(1).value);

    glBindTexture(GL_TEXTURE_2D, id);
    glBegin(GL_QUADS);
    {
      glColor(tint.ToRGB15());
      glTexCoord2i(texcoords[0][0], texcoords[0][1]); glVertex3v16(0, 0, _depth);
      glTexCoord2i(texcoords[1][0], texcoords[1][1]); glVertex2v16(region_width, 0);
      glTexCoord2i(texcoords[2][0], texcoords[2][1]); glVertex2v16(region_width, region_height);
      glTexCoord2i(texcoords[3][0], texcoords[3][1]); glVertex2v16(0, region_height);
    }
    glEnd();
    glPopMatrix(1);
    _depth++;
  }
}