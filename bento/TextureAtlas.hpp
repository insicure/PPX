#pragma once

#include "Texture.hpp"
#include "TextureMap.hpp"
#include <cstdint>
#include <cstdio>

namespace ppx
{
  struct TextureAtlas
  {
    struct TexturePages
    {
      Texture *texture = nullptr;
      TextureMap *images = nullptr;
      uint16_t length = 0;
    };
  
    TexturePages *pageTexture = nullptr;
    uint8_t pages = 0;
    
    TextureMap *find(const char *name);
    TextureMap *operator[](const char *name);
  };

  TextureAtlas *Load_TextureAtlas(const char* filename);
  void Unload_TextureAtlas(TextureAtlas *&ptr);
}