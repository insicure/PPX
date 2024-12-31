#include "bento/palette_loader.hpp"
#include "nds/arm9/sassert.h"
#include "nds/arm9/videoGL.h"
#include "bento/struct.hpp"
#include "bento/utils.hpp"
#include "bento/file.hpp"
#include <cstddef>
#include <sys/_types.h>

// most ds opengl-ish ignore a few arguments, this make me easier to spot one
#define IGNORED 0

namespace nb
{
  static Palette_loader pal(10);

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

  int Texture::Load(const char *fileImage)
  {
    sillyimg_metadata image_metadata;
    size_t image_size;
    unsigned char* image = LoadFileData(fileImage, image_size);
    sassert(image != nullptr, "image not found!");

    image_metadata = *(sillyimg_metadata*)image;

    GL_TEXTURE_TYPE_ENUM type = GL_TEXTURE_TYPE_ENUM::GL_NOTEXTURE;
    const int param = GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|GL_TEXTURE_COLOR0_TRANSPARENT|TEXGEN_OFF;

    switch (image_metadata.format) {
      case 1:
        type = GL_RGBA;
        break;

      case 2:
        type = GL_RGB4;
        break;

      case 3:
        type = GL_RGB16;
        break;

      case 4:
        type = GL_RGB256;
        break;

      case 5:
        type = GL_RGB32_A3;
        break;

      case 6:
        type = GL_RGB8_A5;
        break;

      default:
        sassert(false, "invalid texture format!");
        return -1;
    }

    // TraceLog("header: %llu", image_metadata.header);
    // TraceLog("version: %u", image_metadata.version);
    // TraceLog("format: %u", image_metadata.format);
    // TraceLog("paletteId: %u", image_metadata.paletteId);
    // TraceLog("width: %u", image_metadata.width);
    // TraceLog("height: %u", image_metadata.height);
    // TraceLog("compression: %u", image_metadata.compression);
    // TraceLog("length: %u", image_metadata.length);

    int res = glGenTextures(1, &id);
    sassert(res == 1, "failed gen texture");

    res = glBindTexture(IGNORED, id);
    sassert(res == 1, "failed bind texture");
    
    res = glTexImage2D(IGNORED, IGNORED, type, image_metadata.width, image_metadata.height, IGNORED, param, (image+20));
    sassert(res == 1, "failed glTexImage2D");

    Palette *palette = pal.Get(image_metadata.paletteId);
    sassert(palette != nullptr, "palette not found!");
    TraceLog("load texture %u with pid %u,%u", id, palette->pid, palette->texid);

    // rebind
    res = glBindTexture(IGNORED, id);
    sassert(res == 1, "failed bind texture");

    glAssignColorTable(IGNORED, palette->texid);

    width = image_metadata.width;
    height = image_metadata.height;
    free(image);
    return 0;
  }

  bool Texture::isValid()
  {
    return (this->id > 0) && (this->width > 0) && (this->height > 0);
  }

}