#include "nds/arm9/videoGL.h"
#include "bento/struct.hpp"
#include "bento/utils.hpp"

// most ds opengl-ish ignore a few arguments, this make me easier to spot one
#define IGNORED 0


namespace nb
{
  Texture::Texture(const char *fileImage, const char *filePalette, const int width, const int height, const ImageType format)
    : id(0), width(0), height(0)
  {
    Load(fileImage, filePalette, width, height, format);
  }

  Texture::Texture(const Image &image, const Image &palette)
    : id(0), width(0), height(0)
  {
    Load(image, palette);
  }

  void Texture::Unload()
  {
    if (glDeleteTextures(1, &this->id) == 1)
    {
      TraceLog("tex io: unloaded %i", this->id);
      this->id = 0;
      this->width = 0;
      this->height = 0;
    }
    else TraceLog("tex io: fail unload %i", this->id);
  }

  int Texture::Load(const char *fileImage, const char *filePalette, const int width, const int height, const ImageType format)
  {
    Image image;
    Image palette;

    if (image.Load(fileImage, width, height, format) == -1)
    {
      TraceLog("tex io: open image failed %s", fileImage);
      return -1;
    }

    if (format != ImageType_INDEXED_256)
    {
      if (palette.Load(filePalette, 0, 0, ImageType::ImageType_PALETTE_16) == -1)
      {
        TraceLog("tex io: open palette failed %s", fileImage);
        return -1;
      }
    }
    
    return Load(image, palette);
  }

  int Texture::Load(const Image &image, const Image &palette)
  {
    GL_TEXTURE_TYPE_ENUM type = GL_TEXTURE_TYPE_ENUM::GL_NOTEXTURE;
    const int param = GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|GL_TEXTURE_COLOR0_TRANSPARENT|TEXGEN_OFF;

    switch (image.format) {
      case ImageType_R8G8B8A8:
        // TODO: convert R8G8B8A8 into R5G5B5A1
        sassert(false, "invalid texture format!");
        return -1;

      case ImageType_R5G5B5A1:
        type = GL_TEXTURE_TYPE_ENUM::GL_RGBA;
        break;

      case ImageType_INDEXED_4:
        type = GL_TEXTURE_TYPE_ENUM::GL_RGB4;
        break;

      case ImageType_INDEXED_16:
        type = GL_TEXTURE_TYPE_ENUM::GL_RGB16;
        break;

      case ImageType_INDEXED_256:
        type = GL_TEXTURE_TYPE_ENUM::GL_RGB256;
        break;

      case ImageType_INDEXED_32_A3:
        type = GL_TEXTURE_TYPE_ENUM::GL_RGB32_A3;
        break;

      case ImageType_INDEXED_8_A5:
        type = GL_TEXTURE_TYPE_ENUM::GL_RGB8_A5;
        break;

      case ImageType_PALETTE_16:
        sassert(false, "invalid texture format!");
        return -1;

      case INVALID:
        sassert(false, "invalid texture format!");
        return -1;
    }

    // generate texture
    {
      if (glGenTextures(1, &this->id) == 0)
      {
        TraceLog("tex io: failed to generate texture");
        return -1;
      }

      if (glBindTexture(IGNORED, this->id) == 0)
      {
        TraceLog("tex io: failed to bind texture");
        return -1;
      }
    }

    // generate TexImage2D
    {
      int result = glTexImage2D(IGNORED, IGNORED, type, image.width, image.height, IGNORED, param, image.data);
      if (result == 0)
      {
        TraceLog("tex io: failed to initialize texture");
        return -1;
      }
    }

    // generate ColorTableEXT
    if (image.format != ImageType::ImageType_R5G5B5A1)
    {
      int result = glColorTableEXT(IGNORED, IGNORED, palette.width, IGNORED, IGNORED, palette.data);

      if (result == 0)
      {
        TraceLog("tex io: failed to initialize palette");
        return -1;
      }
    }

    TraceLog("tex io: loaded %i", this->id);
    this->width = image.width;
    this->height = image.height;

    return 0;
  }

  bool Texture::isValid()
  {
    return (this->id > 0) && (this->width > 0) && (this->height > 0);
  }

}