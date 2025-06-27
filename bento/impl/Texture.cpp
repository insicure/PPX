// most ds opengl-ish ignore a few arguments, this make me easier to spot one
#include "bento/Texture.hpp"
#include "bento/Color.hpp"
#include "bento/SillyImage.hpp"
#include "bento/Tracelog.hpp"
#include "nds/arm9/videoGL.h"
#include <cstdint>

constexpr int IGNORED = 0;
extern s32 _depth;

namespace ppx
{
  // simple check if size is power of 2
  static bool isTextureDimensionValid(uint16_t size) {
    return (size >= 8 && size <= 1024) && !(size & (size - 1));
  }

  Texture::Texture(const SillyImage &image) {
    if (!Load(image)) {
      TraceLog("Texture: failed to create from SillyImage");
    }
  }

  Texture::Texture(const char *filename) {
    if (!Load(filename)) {
      TraceLog("Texture: failed to load '%s'", filename);
    }
  }

  bool Texture::Load(const SillyImage &image)
  {
    if (!image.isValid()) {
      TraceLog("Texture: source image invalid");
      return false;
    }

    if (!isTextureDimensionValid(image.width) || !isTextureDimensionValid(image.height)) {
      TraceLog("Texture: invalid dimensions %ux%u", image.width, image.height);
      return false;
    }

    GL_TEXTURE_TYPE_ENUM gl_type = GL_TEXTURE_TYPE_ENUM::GL_NOTEXTURE;
    constexpr int gl_param = GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|GL_TEXTURE_COLOR0_TRANSPARENT|TEXGEN_OFF;

    // find texture type
    switch (image.format)
    {
      case ImageType_R5G5B5A1: gl_type = GL_TEXTURE_TYPE_ENUM::GL_RGBA; break;
      case ImageType_INDEXED_4: gl_type = GL_TEXTURE_TYPE_ENUM::GL_RGB4; break;
      case ImageType_INDEXED_16: gl_type = GL_TEXTURE_TYPE_ENUM::GL_RGB16; break;
      case ImageType_INDEXED_256: gl_type = GL_TEXTURE_TYPE_ENUM::GL_RGB256; break;
      case ImageType_INDEXED_32_A8: gl_type = GL_TEXTURE_TYPE_ENUM::GL_RGB32_A3; break;
      case ImageType_INDEXED_8_A32: gl_type = GL_TEXTURE_TYPE_ENUM::GL_RGB8_A5; break;

      default:
      {
        TraceLog("Texture: invalid format %u", image.format);
        return false;
      };
    }

    if (glGenTextures(1, &id) != 1) {
      TraceLog("Texture: glGenTextures failed");
      return false;
    }

    if (glBindTexture(IGNORED, id) != 1) {
      TraceLog("Texture: glBindTexture failed");
      Unload();
      return false;
    }

    if (glTexImage2D(IGNORED, IGNORED, gl_type, 
                     image.width, image.height, IGNORED, 
                     gl_param, image.data) != 1) 
    {
      TraceLog("Texture: glTexImage2D failed");
      Unload();
      return false;
    }

    if (image.format == ImageType_INDEXED_256 ||
        image.format == ImageType_INDEXED_16 ||
        image.format == ImageType_INDEXED_4 ||
        image.format == ImageType_INDEXED_32_A8 ||
        image.format == ImageType_INDEXED_8_A32)
    {
      if (glColorTableEXT(IGNORED, IGNORED, 
                          image.palette_count, IGNORED, 
                          IGNORED, image.palette_data) != 1) 
      {
        TraceLog("Texture: glColorTableEXT failed");
        Unload();
        return false;
      }
    }

    width = image.width;
    height = image.height;
    TraceLog("Texture: loaded %ux%u id:%u", width, height, id);
    return true;
  }

  bool Texture::Load(const char *filename)
  {
    SillyImage img;
    if (!img.Load(filename)) return false;
    return Load(img);
  }

  void Texture::Unload()
  {
    if (!isValid()) return;

    const int temp_id = id;
    if (glDeleteTextures(1, &id) == 1) {
      TraceLog("Texture: unloaded id:%i", temp_id);
      id = width = height = 0;
    } else {
      TraceLog("Texture: failed to unload id:%i", temp_id);
    }
  }

  bool Texture::isValid()
  {
    return (width > 0) && (height > 0);
  }

  void Texture::Draw(const Vec2 &position, const Vec2 &scale, const Vec2 &origin, int rotation, bool flip_x, bool flip_y, const Rect &region, const Color tint)
  {
    // Default to full texture size if not specified
    const int region_width = ((region.width > 0) ? region.width.toInt() : width);
    const int region_height = ((region.height > 0) ? region.height.toInt() : height);

    glPushMatrix();
    glTranslatef32(position.x.toInt(), position.y.toInt(), 0);
    if (rotation != 0) glRotateZi(degreesToAngle(rotation));
    glTranslatef32(-origin.x.toInt(), -origin.y.toInt(), 0);
    glScalef32(scale.x.value, scale.y.value, f32(1).value);

    glBindTexture(GL_TEXTURE_2D, id);
    glBegin(GL_QUADS);
    {
      glColor(tint.ToRGB15());
      // Calculate texcoords with flip support
      const int tx1 = (flip_x ? region.x + region_width : region.x).toInt();
      const int tx2 = (flip_x ? region.x : region.x + region_width).toInt();
      const int ty1 = (flip_y ? region.y + region_height : region.y).toInt();
      const int ty2 = (flip_y ? region.y : region.y + region_height).toInt();

      glTexCoord2i(tx1, ty1); glVertex3v16(0, 0, _depth);
      glTexCoord2i(tx2, ty1); glVertex2v16(region_width, 0);
      glTexCoord2i(tx2, ty2); glVertex2v16(region_width, region_height);
      glTexCoord2i(tx1, ty2); glVertex2v16(0, region_height);
    }
    glEnd();
    glPopMatrix(1);
    _depth++;
  }
}