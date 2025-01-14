#include "../TextureAtlas.hpp"
#include "../ErrorEnum.hpp"
#include "../external/murmurhash.h"
#include "../Tracelog.hpp"
#include "nds/arm9/sassert.h"
#include <cstring>
#include <new>

#define ATLAS_HEADER 0x68637263

namespace ppx
{
  TextureAtlas::TextureAtlas(const char* filename)
  {
    Load(filename);
  }


  int TextureAtlas::Load(const char* filename)
  {
    FILE *file = nullptr;
    int error = 0;

    while(true)
    {
      file = fopen(filename, "rb");
      sassert(file != nullptr, "TextureAtlas: fopen failed");
      if (file == nullptr)
      {
        error = Error_TextureAtlas_fopenFailed;
        break;
      }

      // check header
      {
        uint32_t header;
        fread(&header, sizeof(uint32_t), 1, file);
        sassert(header == ATLAS_HEADER, "TextureAtlas: invalid header");
        if (header != ATLAS_HEADER)
        {
          error = Error_TextureAtlas_InvalidHeader;
          break;
        }
      }

      // check version
      {
        int16_t version;
        fread(&version, sizeof(int16_t), 1, file);
        sassert(version == 0, "TextureAtlas: invalid version");
        if (version != 0)
        {
          error = Error_TextureAtlas_InvalidVersion;
          break;
        }
      }

      bool enable_trim = false;
      bool enable_rotate = false;
      uint8_t string_type = 0;

      fread(&enable_trim, sizeof(uint8_t), 1, file);
      fread(&enable_rotate, sizeof(uint8_t), 1, file);
      fread(&string_type, sizeof(uint8_t), 1, file);

      sassert(enable_trim && enable_rotate, "TextureAtlas: Trim/Rotate disabled");
      if (!enable_trim || !enable_rotate)
      {
        error = Error_TextureAtlas_TrimRotateDisabled;
        break;
      }

      sassert(string_type == 0, "TextureAtlas: invalid string type");
      if (string_type != 0)
      {
        error = Error_TextureAtlas_InvalidStringType;
        break;
      }

      fread(&numTexture, sizeof(int16_t), 1, file);
      texture = new(std::nothrow) Texture[numTexture];
      sassert(texture != nullptr, "TextureAtlas: allocate texture failed");
      if (texture == nullptr)
      {
        error = Error_TextureAtlas_AllocateFailed;
        break;
      }

      map = new(std::nothrow) TextureMap*[numTexture];
      sassert(map != nullptr, "TextureAtlas: allocate map failed");
      if (map == nullptr)
      {
        error = Error_TextureAtlas_AllocateFailed;
        break;
      }

      numMap = new(std::nothrow) int16_t[numTexture];
      sassert(numMap != nullptr, "TextureAtlas: allocate numMap failed");
      if (numMap == nullptr)
      {
        error = Error_TextureAtlas_AllocateFailed;
        break;
      }

      for (int i=0; i<numTexture; i++)
      {
        map[i] = nullptr;
        numMap[i] = 0;
      }

      for (int i_tex=0; i_tex<numTexture; i_tex++)
      {
        if (error != Error_OK) break;

        char texture_name[50];
        char path[128];
        readString(file, texture_name);
        sprintf(path, "nitro:/texturemap/%s_img.bin", texture_name);

        error = texture[i_tex].Load(path);
        if (error != Error_OK) break;

        fread(&numMap[i_tex], sizeof(int16_t), 1, file);
        map[i_tex] = new(std::nothrow) TextureMap[numMap[i_tex]];
        sassert(map[i_tex] != nullptr, "TextureAtlas: allocate TextureMap failed");
        if (map[i_tex] == nullptr)
        {
          error = Error_TextureAtlas_AllocateFailed;
          break;
        }

        for (int i_img=0; i_img<numMap[i_tex]; i_img++)
        {
          char image_name[128];
          readString(file, image_name);

          map[i_tex][i_img].id = texture[i_tex].id;
          map[i_tex][i_img].hash = murmurhash(image_name, strlen(image_name), 0);

          fread(&map[i_tex][i_img].frame_x, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].frame_y, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].frame_width, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].frame_height, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].offset_x, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].offset_y, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].width, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].height, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].rotated, sizeof(uint8_t), 1, file);

          // TraceLog("%s(%u), %i %i %i %i, %i %i %i %i", image_name, map[i_tex][i_img].hash, map[i_tex][i_img].frame_x, map[i_tex][i_img].frame_y, map[i_tex][i_img].width, map[i_tex][i_img].height, map[i_tex][i_img].offset_x, map[i_tex][i_img].offset_y, map[i_tex][i_img].frame_width, map[i_tex][i_img].frame_height);
        }

      }

      break;
    }

    if (file != nullptr) fclose(file);
    if (error != Error_OK) Unload();
    else TraceLog("TextureAtlas: loaded %s", filename);

    return error;
  }

  void TextureAtlas::Unload()
  {
    if (texture != nullptr)
    {
      for (int i=0; i<numTexture; i++)
        texture[i].Unload();

      delete[] texture;
    }

    if (map != nullptr)
    {
      for (int i=0; i<numTexture; i++)
        if (map[i] != nullptr) delete[] map[i];

      delete[] map;
    }

    delete[] numMap;

    texture = nullptr;
    map = nullptr;
    numMap = nullptr;
    numTexture = 0;
  }

  bool TextureAtlas::isValid()
  {
    return (texture != nullptr && map != nullptr);
  }

  TextureMap *TextureAtlas::operator[](const char *name)
  {
    // find texturemap
    uint32_t hash = murmurhash(name, strlen(name), 0);

    for (int i_tex=0; i_tex<numTexture; i_tex++)
    {
      for (int i_img=0; i_img<numMap[i_tex]; i_img++)
      {
        if (hash == map[i_tex][i_img].hash)
          return &map[i_tex][i_img];
      }
    }
    return nullptr;
  }

  void TextureAtlas::readString(FILE *file, char *buffer)
  {
    // null terminated string type
    char c;
    for(int i=0; i<256; i++)
    {
      fread(&c, sizeof(char), 1, file);
      if (buffer != nullptr) buffer[i] = c;
      
      if (c == '\0') break;
    }
  }
}