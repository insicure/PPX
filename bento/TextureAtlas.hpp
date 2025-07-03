#pragma once

#include "Texture.hpp"
#include "TextureMap.hpp"
#include <cstdint>
#include <cstdio>

namespace ppx
{
  class TextureAtlas
  {
  private:
    struct TextureAtlasItem
    {
      Texture *texture = nullptr;
      TextureMap *images = nullptr;
      uint16_t length = 0;
    };
  
    TextureAtlas() = default;
    TextureAtlas& operator=(const TextureAtlas&) = delete;

  public:
    ~TextureAtlas() { Unload(); }
    TextureAtlasItem *items = nullptr;
    uint8_t length = 0;

    static TextureAtlas *Load(const char* filename);
    void Unload();
    bool isValid();
    TextureMap *operator[](const char *name);
  
  };
}