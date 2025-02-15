#pragma once

#include "Texture.hpp"
#include "TextureMap.hpp"
#include <cstdint>
#include <cstdio>

namespace ppx
{
  struct TextureAtlasItem
  {
    Texture *texture = nullptr;
    TextureMap *images = nullptr;
    uint16_t length = 0;
  };

  class TextureAtlas
  {
  public:
    TextureAtlasItem *items = nullptr;
    uint8_t length = 0;

    constexpr TextureAtlas() : items(nullptr), length(0) {}
    TextureAtlas(const char* filename);

    bool Load(const char* filename);
    void Unload();
    bool isValid();
    TextureMap *operator[](const char *name);
  };
}