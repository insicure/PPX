#include "../TextureAtlas.hpp"
#include "../Assert.hpp"
#include "../external/murmurhash.h"
#include "../Tracelog.hpp"
#include "bento/Texture.hpp"
#include "bento/TextureMap.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <new>

#define ATLAS_HEADER 0x68637263

namespace ppx
{
  TextureAtlas::TextureAtlas(const char* filename)
  {
    Load(filename);
  }

  bool TextureAtlas::Load(const char * filename)
  {
    FILE *file = nullptr;
    bool res = false;

    do
    {
      file = fopen(filename, "rb");
      res_sassert(res, file != nullptr, "TextureAtlas: fopen failed, %s", filename);
      if (!res) break;

      char line[128];
      
      // check header
      {
        char *ptr = fgets(line, sizeof(line), file);
        res_sassert(res, ptr != nullptr, "TextureAtlas: invalid header, %s", filename);
        if(!res) break;

        int count = sscanf(line, "sillyatl %hhu", &length);
        res_sassert(res, count == 1, "TextureAtlas: invalid header, %s", filename);
        if(!res) break;
      }

      items = new(std::nothrow) TextureAtlasItem[length];
      res_sassert(res, items != nullptr, "TextureAtlas: allocate TextureAtlasItem failed, %s", filename);
      if(!res) break;

      for (int i_tex = 0; i_tex<length; i_tex++)
      {
        TextureAtlasItem &item = items[i_tex];
        // get texture name and images length
        char texture_name[64];
        char path[128];

        char *ptr = fgets(line, sizeof(line), file);
        res_sassert(res, ptr != nullptr, "TextureAtlas: invalid format, %s", filename);
        if(!res) break;

        int count = sscanf(line, "%s %hu", texture_name, &item.length);
        res_sassert(res, count == 2, "TextureAtlas: invalid format, %s", filename);
        if(!res) break;

        snprintf(path, sizeof(path), "nitro:/atlas/%s.sillyimg", texture_name);

        item.texture = new(std::nothrow) Texture();
        res_sassert(res, item.texture != nullptr, "TextureAtlas: allocate Texture failed, %s", filename);
        if (!res) break;

        res = item.texture->Load(path);
        if (!res) break;

        item.images = new(std::nothrow) TextureMap[item.length];
        res_sassert(res, item.images != nullptr, "TextureAtlas: allocate TextureMap failed, %s", filename);
        if (!res) break;

        // get images
        for (int i_img=0; i_img<item.length; i_img++)
        {
          TextureMap &image = item.images[i_img];

          char image_name[64];
          uint16_t temp_rotated = 0;

          char *ptr = fgets(line, sizeof(line), file);
          res_sassert(res, ptr != nullptr, "TextureAtlas: invalid format, %s", filename);
          if(!res) break;

          int count = sscanf(line, "%s %hu %hu %hu %hu %hu %hu %hu %hu %hu",
                              image_name,
                              &image.frame_x,
                              &image.frame_y,
                              &image.frame_width,
                              &image.frame_height,
                              &image.offset_x,
                              &image.offset_y,
                              &image.width,
                              &image.height,
                              &temp_rotated);
          res_sassert(res, count == 10, "TextureAtlas: invalid format, %s", filename);
          if(!res) break;

          image.rotated = (temp_rotated == 1);
          image.id = item.texture->id;
          image.hash = murmurhash(image_name, strlen(image_name), 0);
        }
        if (!res) break;

      }
      if (!res) break;

    } while(0);

    if (file != nullptr) fclose(file);
    if (!res)
    {
      TraceLog("TextureAtlas: load failed,  %s", filename);
      Unload();
    }
    else TraceLog("TextureAtlas: loaded, %s", filename);

    return res;
  }

  void TextureAtlas::Unload()
  {
    if (isValid())
    {
      for (int i_tex = 0; i_tex<length; i_tex++)
      {
        items[i_tex].texture->Unload();
        
        delete items[i_tex].texture;
        delete[] items[i_tex].images;
      }

      delete[] items; 
    }

    items = nullptr;
    length = 0;
  }

  bool TextureAtlas::isValid()
  {
    return (items != nullptr);
  }

  TextureMap *TextureAtlas::operator[](const char *name)
  {
    // find texturemap
    uint32_t hash = murmurhash(name, strlen(name), 0);

    for (int i_tex=0; i_tex<length; i_tex++)
    {
      TextureAtlasItem &item = items[i_tex];

      for (int i_img=0; i_img<item.length; i_img++)
      {
        if (hash == item.images[i_img].hash)
          return &item.images[i_img];
      }
    }
    return nullptr;
  }
}