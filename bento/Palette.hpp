#pragma once

#include <cstdint>

namespace ppx
{
  class Palette
  {
  public:

    uint8_t pid = 0;
    int texid = -1;

    constexpr Palette() : pid(0), texid(-1) { }
    bool isValid();

    static int Get(Palette *palette, const int paletteId); 
  };
}