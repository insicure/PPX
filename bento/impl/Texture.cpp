#include "../Texture.hpp"
#include "../ErrorEnum.hpp"
#include "../Palette.hpp"
#include "../Rect.hpp"
#include "../SillyImage.hpp"
#include "../Tracelog.hpp"
#include "../Math.hpp"
#include "nds/arm9/sassert.h"
#include "nds/arm9/videoGL.h"
#include <cstdio>

// most ds opengl-ish ignore a few arguments, this make me easier to spot one
#define IGNORED 0

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


  int Texture::Load(const SillyImage &image)
  {
    int error = Error_OK;

    // find palette
    Palette *palette = nullptr;
    {
      if (image.format == ImageType_INDEXED_4 ||
          image.format == ImageType_INDEXED_16 ||
          image.format == ImageType_INDEXED_256 ||
          image.format == ImageType_INDEXED_32_A3 ||
          image.format == ImageType_INDEXED_32_A3)
      {
        error = Palette::Get(palette, image.paletteId);
        if (error != Error_OK)
          return error;
      }
    }

    GL_TEXTURE_TYPE_ENUM type = GL_TEXTURE_TYPE_ENUM::GL_NOTEXTURE;
    const int param = GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|GL_TEXTURE_COLOR0_TRANSPARENT|TEXGEN_OFF;

    switch (image.format)
    {
      case ImageType_R8G8B8A8:
        // TODO: convert R8G8B8A8 into R5G5B5A1
        sassert(false, "Texture: invalid texture format!");
        return Error_Texture_InvalidFormat;

      case ImageType_R5G5B5A1: type = GL_TEXTURE_TYPE_ENUM::GL_RGBA; break;
      case ImageType_INDEXED_4: type = GL_TEXTURE_TYPE_ENUM::GL_RGB4; break;
      case ImageType_INDEXED_16: type = GL_TEXTURE_TYPE_ENUM::GL_RGB16; break;
      case ImageType_INDEXED_256: type = GL_TEXTURE_TYPE_ENUM::GL_RGB256; break;
      case ImageType_INDEXED_32_A3: type = GL_TEXTURE_TYPE_ENUM::GL_RGB32_A3; break;
      case ImageType_INDEXED_8_A5: type = GL_TEXTURE_TYPE_ENUM::GL_RGB8_A5; break;

      case ImageType_PALETTE_16:
        sassert(false, "Texture: invalid texture format!");
        return Error_Texture_InvalidFormat;

      case ImageType_INVALID:
        sassert(false, "Texture: invalid texture format!");
        return Error_Texture_InvalidFormat;
    }

    // generate texture
    {
      int res = glGenTextures(1, &id);
      sassert(res == 1, "Texture: glGenTextures failed");
      if (res != 1) return Error_Texture_GenTexturesFailed;
    }

    // bind texture
    {
      int res = glBindTexture(IGNORED, id);
      sassert(res == 1, "Texture: glBindTexture failed");
      if (res != 1) return Error_Texture_BindTexturesFailed;
    }

    // generate TexImage2D
    {
      int res = glTexImage2D(IGNORED, IGNORED, type, image.width, image.height, IGNORED, param, image.data);
      sassert(res == 1, "Texture: glTexImage2D failed");
      if (res != 1) return Error_Texture_TexImage2DFailed;
    }

    // assign palette
    if (palette)
    {
      int res = glAssignColorTable(IGNORED, palette->texid);
      sassert(res == 1, "Texture: glAssignColorTable failed");
      if (res != 1) return Error_Texture_AssignPaletteFailed;
    }

    if (palette)
      TraceLog("Texture: loaded id:%u pid:%u", id, palette->pid);
    else
      TraceLog("Texture: loaded id:%u", id);

    width = image.width;
    height = image.height;

    return Error_OK;
  }

  int Texture::Load(const char *filename)
  {
    SillyImage image;
    int error = image.Load(filename);
    if (error != Error_OK)
      return error;

    error = Load(image);
    image.Unload();

    return error;
  }

  void Texture::Unload()
  {
    if (glDeleteTextures(1, &id) == 1)
    {
      TraceLog("Texture: unload id:%i", id);
      id = 0;
      width = 0;
      height = 0;
    }
    else TraceLog("Texture: failed unload id:%i", id);
  }

  bool Texture::isValid()
  {
    return (id > 0) && (width > 0) && (height > 0);
  }

  void Texture::Draw(const Vec2 &position, const int transform)
  {
    Draw(
      Rect(0, 0, width, height),
      Rect(position.x, position.y, width, height),
      transform,
      0,
      Vec2(0, 0)
    );
  }

  void Texture::Draw(const Vec2 &position, const int transform, const int rotation, const Vec2 &scale, const Vec2 origin)
  {
    Draw(
      Rect(0, 0, width, height),
      Rect(position.x, position.y, width*scale.x, height*scale.y),
      transform,
      rotation,
      origin
    );
  }

  void Texture::Draw(const Rect &region, const Rect &dest, const int transform, const int rotation, const Vec2 &origin)
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