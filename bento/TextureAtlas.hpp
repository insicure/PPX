#pragma once

#include "Texture.hpp"
#include "TextureMap.hpp"
#include <cstdio>

namespace ppx
{
  class TextureAtlas
  {
  public:
    Texture *texture;
    TextureMap **map;

    int numTexture;
    int16_t *numMap;

    constexpr TextureAtlas() : texture(nullptr), map(nullptr), numTexture(0), numMap(0) {}
    TextureAtlas(const char* filename);

    int Load(const char* filename);
    void Unload();
    bool isValid();
    TextureMap *operator[](const char *name);

  private:
    void readString(FILE *file, char *buffer);
  };
}