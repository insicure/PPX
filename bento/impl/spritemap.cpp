#include "bento/struct.hpp"
#include "bento/utils.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

namespace nb
{
  SpriteMap::SpriteMap(const char* filename)
  {
    Load(filename);
  }

  SpriteMap::~SpriteMap()
  {

  }

  int SpriteMap::Load(const char* filename)
  {
    Unload();

    FILE *file = fopen(filename, "rb");
    if (file == nullptr)
    {
      TraceLog("spritemap: failed to load %s", filename);
      Unload();
      fclose(file);
      return -1;
    }

    // check header
    {
      uint32_t buffer;
      fread(&buffer, sizeof(uint32_t), 1, file);

        TraceLog("spritemap: header %i", buffer);
        
      if (buffer != 0x68637263)
      {
        TraceLog("spritemap: invalid header %s", filename);
        Unload();
        fclose(file);
        return -1;
      }
    }

    // check version
    {
      int16_t buffer;
      fread(&buffer, sizeof(int16_t), 1, file);

      if (buffer != 0)
      {
        TraceLog("spritemap: invalid version %s", filename);
        Unload();
        fclose(file);
        return -1;
      }
    }

    // read trim & rotated
    fread(&trim_enable, sizeof(uint8_t), 1, file);
    fread(&rotate_enable, sizeof(uint8_t), 1, file);

    // get string type
    uint8_t stringType;
    fread(&stringType, sizeof(uint8_t), 1, file);

    // get length
    fread(&length, sizeof(int16_t), 1, file);

    // allocate memory
    map = (SpriteTex*)malloc(length * sizeof(SpriteTex));
    if (map == nullptr)
    {
      TraceLog("spritemap: allocate SpriteTex failed %s", filename);
      Unload();
      fclose(file);
      return -1;
    }

    // loop SpriteTex
    for (int i=0; i<length; i++)
    {
      SpriteTex &tex = map[i];

      // read texture name
      readString(file);

      // load texture here

      // read texture length
      fread(&tex.length, sizeof(int16_t), 1, file);

      TraceLog("tex length: %i", tex.length);

      // allocate memory
      tex.map = (SpriteImg*)malloc(tex.length * sizeof(SpriteImg));
      if (tex.map == nullptr)
      {
        TraceLog("spritemap: allocate SpriteImg failed %s", filename);
        Unload();
        fclose(file);
        return -1;
      }

      for (int i2=0; i2<tex.length; i2++)
      {
        SpriteImg &img = tex.map[i2];

        // read image name
        readString(file);

        // read image properties
        fread(&img.x, sizeof(int16_t), 1, file);
        fread(&img.y, sizeof(int16_t), 1, file);
        fread(&img.width, sizeof(int16_t), 1, file);
        fread(&img.height, sizeof(int16_t), 1, file);

        // optional trim
        if (trim_enable)
        {
          fread(&img.frame_x, sizeof(int16_t), 1, file);
          fread(&img.frame_y, sizeof(int16_t), 1, file);
          fread(&img.frame_width, sizeof(int16_t), 1, file);
          fread(&img.frame_height, sizeof(int16_t), 1, file);
        }

        // optional rotate
        if (rotate_enable)
          fread(&img.rotated, sizeof(int8_t), 1, file);
      }
    }

    fclose(file);
    return 0;
  }

  bool SpriteMap::isValid()
  {
    return (map != nullptr && length > 0);
  }

  void SpriteMap::readString(FILE *file)
  {
    readString(file, nullptr);
  }

  void SpriteMap::readString(FILE *file, char *buffer)
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

  void SpriteMap::Unload()
  {
    if (map != nullptr)
      for (int i=0; i<length; i++) free(map[i].map);
    
    free(map);

    length = 0;
    map = nullptr;
  }

}