#include "../Texture.hpp"
#include "../Palette.hpp"
#include "../Rect.hpp"
#include "../SillyImage.hpp"
#include "../Tracelog.hpp"
#include "../Assert.hpp"
#include "../Math.hpp"
#include "nds/arm9/videoGL.h"
#include <cassert>
#include <cstdio>

// most ds opengl-ish ignore a few arguments, this make me easier to spot one
#define IGNORED 0

// simple check if size is power of 2
#define IS_POWER2(num) ((num >= 8 && num <= 1024) && !(num & (num - 1)))

extern s32 _depth;

namespace ppx
{

  Texture::Texture(const SillyImage &image)
    : id(0), width(0), height(0)
  {
    Load(image);
  }

  Texture::Texture(const char *filename)
    : id(0), width(0), height(0)
  {
    Load(filename);
  }


  bool Texture::Load(const SillyImage &image)
  {
    bool res = false;
    Palette *palette = nullptr;
    GL_TEXTURE_TYPE_ENUM gl_type = GL_TEXTURE_TYPE_ENUM::GL_NOTEXTURE;
    const int gl_param = GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|GL_TEXTURE_COLOR0_TRANSPARENT|TEXGEN_OFF;

    do
    {
      // verify image dimension
      res_sassert(res, IS_POWER2(image.width) && IS_POWER2(image.height), "Texture: invalid dimension, (%ux%u)", image.width, image.height);
      if (!res) break;

      // find palette
      {
        if (image.format == ImageType_INDEXED_4 ||
            image.format == ImageType_INDEXED_16 ||
            image.format == ImageType_INDEXED_256 ||
            image.format == ImageType_INDEXED_32_A3 ||
            image.format == ImageType_INDEXED_32_A3)
        {
          palette = Palette::Get(image.paletteid);
          res_sassert(res, palette != nullptr, "Texture: couldnt find palette, pid:%u", image.paletteid);
          if (!res) break;
        }
      }

      // find texture type
      switch (image.format)
      {
        case ImageType_R5G5B5A1: gl_type = GL_TEXTURE_TYPE_ENUM::GL_RGBA; break;
        case ImageType_INDEXED_4: gl_type = GL_TEXTURE_TYPE_ENUM::GL_RGB4; break;
        case ImageType_INDEXED_16: gl_type = GL_TEXTURE_TYPE_ENUM::GL_RGB16; break;
        case ImageType_INDEXED_256: gl_type = GL_TEXTURE_TYPE_ENUM::GL_RGB256; break;
        case ImageType_INDEXED_32_A3: gl_type = GL_TEXTURE_TYPE_ENUM::GL_RGB32_A3; break;
        case ImageType_INDEXED_8_A5: gl_type = GL_TEXTURE_TYPE_ENUM::GL_RGB8_A5; break;

        case ImageType_R8G8B8A8: // TODO: convert R8G8B8A8 into R5G5B5A1
        case ImageType_PALETTE_16:
        case ImageType_INVALID:
        default: res = false; break;
      }
      if (!res) break;

      // generate texture
      {
        int r = glGenTextures(1, &id);
        res_sassert(res, r == 1, "Texture: glGenTextures failed");
        if (!res) break;
      }

      // bind texture
      {
        int r = glBindTexture(IGNORED, id);
        res_sassert(res, r == 1, "Texture: glBindTexture failed");
        if (!res) break;
      }

      // generate TexImage2D
      {
        int r = glTexImage2D(IGNORED, IGNORED, gl_type, image.width, image.height, IGNORED, gl_param, image.data);
        res_sassert(res, r == 1, "Texture: glTexImage2D failed");
        if (!res) break;
      }

      // assign palette
      if (palette)
      {
        int r = glAssignColorTable(IGNORED, palette->texid);
        res_sassert(res, r == 1, "Texture: glAssignColorTable failed");
        if (!res) break;
      }
      // if (image.format == ImageType_INDEXED_256 ||
      //     image.format == ImageType_INDEXED_16 ||
      //     image.format == ImageType_INDEXED_4 ||
      //     image.format == ImageType_INDEXED_32_A3 ||
      //     image.format == ImageType_INDEXED_8_A5)
      // {
      //   SillyImage pal_img;

      //   char path[128];
      //   snprintf(path, sizeof(path), "nitro:/palette/%u.sillypal", image.paletteid);

      //   res = pal_img.Load(path);
      //   if (!res) break;

      //   int r = glColorTableEXT(IGNORED, IGNORED, pal_img.width, IGNORED, IGNORED, pal_img.data);
      //   printf("%u\n", r);

      //   pal_img.Unload();
      // }

    } while(0);

    if (!res)
    {
      TraceLog("Texture: load failed");
      Unload();
    } 
    else
    {
      width = image.width;
      height = image.height;
      
      if (palette)
        TraceLog("Texture: load success, (%ux%u) id:%u pid:%u", width, height, id, palette->pid);
      else
        TraceLog("Texture: load success, (%ux%u) id:%u pid:none", width, height, id);
    }

    return res;
  }

  bool Texture::Load(const char *filename)
  {
    bool res = false;
    SillyImage img;

    do
    {
      res = img.Load(filename);
      if (!res) break;

      res = Load(img);
      if (!res) break;

    } while(0);

    img.Unload();
    return res;
  }

  void Texture::Unload()
  {
    if (isValid())
    {
      int temp_id = id;
      if (glDeleteTextures(1, &id) == 1)
      {

        TraceLog("Texture: unload id:%i", temp_id);
        id = -1;
        width = 0;
        height = 0;
      }
      else TraceLog("Texture: failed unload id:%i", temp_id);
    }
  }

  bool Texture::isValid()
  {
    return (width > 0) && (height > 0);
  }

  void Texture::Draw(const Vec2 &position, const int transform) const
  {
    Draw(
      Rect(0, 0, width, height),
      Rect(position.x, position.y, width, height),
      transform,
      0,
      Vec2(0, 0)
    );
  }

  void Texture::Draw(const Vec2 &position, const int transform, const int rotation, const Vec2 &scale, const Vec2 origin) const
  {
    Draw(
      Rect(0, 0, width, height),
      Rect(position.x, position.y, width*scale.x, height*scale.y),
      transform,
      rotation,
      origin
    );
  }

  void Texture::Draw(const Rect &region, const Rect &dest, const int transform, const int rotation, const Vec2 &origin) const
  {
    Vec2 tex_coord[4] = {
      {region.Left(), region.Top()},
      {region.Left(), region.Bottom()},
      {region.Right(), region.Bottom()},
      {region.Right(), region.Top()}
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