#include "nds/arm9/sassert.h"
#include "nds/arm9/videoGL.h"
#include "bento/struct.hpp"
#include "bento/utils.hpp"
#include <cstdint>
#include <sys/_types.h>
#include <vector>

// most ds opengl-ish ignore a few arguments, this make me easier to spot one
#define IGNORED 0


namespace nb
{
  std::vector<Palette> PaletteList;

  Texture::Texture(const Image &image)
    : id(0), width(0), height(0)
  {
    Load(image);
  }

  Texture::Texture(const char *filename)
    : id(0), width(0), height(0)
  {
    Load(filename);
  }

  void Texture::Unload()
  {
    TraceLog("tex io: unloaded %i", id);
    if (glDeleteTextures(1, &id) == 1)
    {
      id = 0;
      width = 0;
      height = 0;
    }
    else TraceLog("tex io: fail unload %i", id);
  }

  int Texture::Load(const Image &image)
  {
    int PaletteIndex = -1;

    // find palette
    {
      if (false
          || image.format == ImageType_INDEXED_4
          || image.format == ImageType_INDEXED_16
          || image.format == ImageType_INDEXED_256
          || image.format == ImageType_INDEXED_32_A3
          || image.format == ImageType_INDEXED_32_A3)
      {
        for (unsigned int i=0; i<PaletteList.size(); i++)
          if (PaletteList[i].pid == image.paletteId)
          {
            PaletteIndex = i;
            break;
          }

        if (PaletteIndex == -1)
        {
          char path[30];

          // TODO: dont hardcode palette directory
          sprintf(path, "%s%u_pal.bin", "nitro:/palette/", image.paletteId);

          Image palette_img;
          Palette palette;

          if (palette_img.Load(path) == -1)
          {
            TraceLog("tex io: failed to load palette %u", image.paletteId);
            return -1;
          }

          // generate palette texture
          {
            if (glGenTextures(1, &palette.texid) == 0)
            {
              TraceLog("tex io: failed to generate palette texture");
              palette_img.Unload();
              return -1;
            }

            if (glBindTexture(IGNORED, palette.texid) == 0)
            {
              TraceLog("tex io: failed to bind palette texture");
              palette_img.Unload();
              return -1;
            }
          }

          // generate ColorTableEXT
          {
            int result = glColorTableEXT(IGNORED, IGNORED, palette_img.width, IGNORED, IGNORED, palette_img.data);

            if (result == 0)
            {
              TraceLog("tex io: failed to generate palette table");
              palette_img.Unload();
              return -1;
            }
          }
          
          palette.pid = palette_img.paletteId;
          PaletteList.push_back(palette);
          PaletteIndex = PaletteList.size() - 1;
          TraceLog("tex io: palette loaded %i, %u", palette.texid, palette.pid);

          palette_img.Unload();
        }
      }
    }

    GL_TEXTURE_TYPE_ENUM type = GL_TEXTURE_TYPE_ENUM::GL_NOTEXTURE;
    const int param = GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|GL_TEXTURE_COLOR0_TRANSPARENT|TEXGEN_OFF;

    switch (image.format) {
      case ImageType_R8G8B8A8:
        // TODO: convert R8G8B8A8 into R5G5B5A1
        sassert(false, "invalid texture format!");
        return -1;

      case ImageType_R5G5B5A1: type = GL_TEXTURE_TYPE_ENUM::GL_RGBA; break;
      case ImageType_INDEXED_4: type = GL_TEXTURE_TYPE_ENUM::GL_RGB4; break;
      case ImageType_INDEXED_16: type = GL_TEXTURE_TYPE_ENUM::GL_RGB16; break;
      case ImageType_INDEXED_256: type = GL_TEXTURE_TYPE_ENUM::GL_RGB256; break;
      case ImageType_INDEXED_32_A3: type = GL_TEXTURE_TYPE_ENUM::GL_RGB32_A3; break;
      case ImageType_INDEXED_8_A5: type = GL_TEXTURE_TYPE_ENUM::GL_RGB8_A5; break;

      case ImageType_PALETTE_16:
        sassert(false, "invalid texture format!");
        return -1;

      case ImageType_INVALID:
        sassert(false, "invalid texture format!");
        return -1;
    }

    // generate texture
    {
      if (glGenTextures(1, &id) == 0)
      {
        TraceLog("tex io: failed to generate texture");
        return -1;
      }

      if (glBindTexture(IGNORED, id) == 0)
      {
        TraceLog("tex io: failed to bind texture");
        return -1;
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

      // assign palette
      {
        if (PaletteIndex != -1)
        {
          if (glAssignColorTable(IGNORED, PaletteList[PaletteIndex].texid) == 0)
          {
            TraceLog("tex io: failed to asign palette");
            return -1;
          }
        }
      }
    }

    TraceLog("tex io: texture loaded %i", id);
    width = image.width;
    height = image.height;

    return 0;
  }

  int Texture::Load(const char *filename)
  {
    Image image;
    int res = image.Load(filename); 
    if (res != 0) return -1;
    
    res = Load(image);
    image.Unload();

    return res;
  }

  bool Texture::isValid()
  {
    return (id > 0) && (width > 0) && (height > 0);
  }

  const TextureFrame Texture::GetFrame() const
  {
    return TextureFrame(0, 0, width, height, false);
  }

  const TextureFrame TextureMap::GetFrame() const
  {
    return TextureFrame(frame_x, frame_y, static_cast<uint16_t>(frame_width), static_cast<uint16_t>(frame_height), rotated);
  }

}