#include "bento/external/murmurhash.h"
#include "bento/struct.hpp"
#include "bento/utils.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace nb
{
  TextureAtlas::TextureAtlas(const char* filename)
  {
    Load(filename);
  }

  TextureAtlas::~TextureAtlas()
  {

  }

  int TextureAtlas::Load(const char* filename)
  {
    FILE *file = nullptr;
    int error = 0;

    while(true)
    {
      file = fopen(filename, "rb");
      if (file == nullptr)
      {
        TraceLog("spritemap: failed to load %s", filename);
        error = -1;
        break;
      }

      // check header
      {
        uint32_t header;
        fread(&header, sizeof(uint32_t), 1, file);

        if (header != 0x68637263)
        {
          TraceLog("spritemap: invalid header %s", filename);
          error = -1;
          break;
        }
      }

      // check version
      {
        int16_t version;
        fread(&version, sizeof(int16_t), 1, file);

        if (version != 0)
        {
          TraceLog("spritemap: invalid version %s", filename);
          error = -1;
          break;
        }
      }

      bool enable_trim = false;
      bool enable_rotate = false;
      uint8_t string_type = 0;

      fread(&enable_trim, sizeof(uint8_t), 1, file);
      fread(&enable_rotate, sizeof(uint8_t), 1, file);
      fread(&string_type, sizeof(uint8_t), 1, file);

      if (!enable_trim || !enable_rotate)
      {
        TraceLog("spritemap: trim/rotate not enabled %s", filename);
        error = -1;
        break;
      }

      if (string_type != 0)
      {
        TraceLog("spritemap: invalid string type %s", filename);
        error = -1;
        break;
      }

      fread(&numTexture, sizeof(int16_t), 1, file);
      texture = (Texture**)malloc(numTexture * sizeof(Texture*));
      if (texture == nullptr)
      {
        TraceLog("spritemap: failed malloc textures %s", filename);
        error = -1;
        break;
      }

      map = (TextureMap**)malloc(numMap * sizeof(TextureMap*));
      if (map == nullptr)
      {
        TraceLog("spritemap: failed malloc maps %s", filename);
        error = -1;
        break;
      }

      for (int i=0; i<numTexture; i++)
      {
        texture[i] = nullptr;
        map[i] = nullptr;
      }

      for (int i_tex=0; i_tex<numTexture; i_tex++)
      {
        if (error != 0) break;

        char texture_name[50];
        char path[128];
        readString(file, texture_name);
        sprintf(path, "nitro:/texturemap/%s_img.bin", texture_name);

        texture[i_tex] = (Texture*)malloc(sizeof(Texture));
        if (texture[i_tex] == nullptr)
        {
          TraceLog("spritemap: failed malloc texture %i %s", i_tex, filename);
          error = -1;
          break;
        }

        if (texture[i_tex]->Load(path) != 0)
        {
          TraceLog("spritemap: failed load texture %i %s", i_tex, filename);
          error = -1;
          break;
        }

        fread(&numMap, sizeof(int16_t), 1, file);
        map[i_tex] = (TextureMap*)malloc(numMap * sizeof(TextureMap));
        if (map[i_tex] == nullptr)
        {
          TraceLog("spritemap: failed malloc map %i %s", i_tex, filename);
          error = -1;
          break;
        }

        for (int i_img=0; i_img<numMap; i_img++)
        {
          char image_name[128];
          readString(file, image_name);

          map[i_tex][i_img].id = texture[i_tex]->id;
          map[i_tex][i_img].hash = murmurhash(image_name, strlen(image_name), 0);
          
          fread(&map[i_tex][i_img].x, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].y, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].frame_width, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].frame_height, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].frame_x, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].frame_y, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].width, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].height, sizeof(int16_t), 1, file);
          fread(&map[i_tex][i_img].rotated, sizeof(uint8_t), 1, file);

          TraceLog("%s(%u), %i %i %i %i, %i %i %i %i", image_name, map[i_tex][i_img].hash, map[i_tex][i_img].x, map[i_tex][i_img].y, map[i_tex][i_img].width, map[i_tex][i_img].height, map[i_tex][i_img].frame_x, map[i_tex][i_img].frame_y, map[i_tex][i_img].frame_width, map[i_tex][i_img].frame_height);
        }

      }
      
      break;
    }

    if (file != nullptr) fclose(file);
    if (error != 0) Unload();
    return error;
  }

  TextureMap *TextureAtlas::Find(const char *name) const
  {
    // find texturemap
    uint32_t hash = murmurhash(name, strlen(name), 0);

    for (int i_tex=0; i_tex<numTexture; i_tex++)
    {
      for (int i_img=0; i_img<numMap; i_img++)
      {
        if (hash == map[i_tex][i_img].hash)
          return &map[i_tex][i_img];
      }
    }

    return nullptr;
  }

  bool TextureAtlas::isValid()
  {
    return (texture != nullptr && map != nullptr);
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

  void TextureAtlas::Unload()
  {
    if (texture != nullptr)
    {
      for (int i=0; i<numTexture; i++)
        if (texture[i] != nullptr) free(texture[i]);
      
      free(texture);
    }

    if (map != nullptr)
    {
      for (int i=0; i<numMap; i++)
        if (map[i] != nullptr) free(map[i]);

      free(map);
    }

    texture = nullptr;
    map = nullptr;
    numTexture = 0;
    numMap = 0;
  }

}