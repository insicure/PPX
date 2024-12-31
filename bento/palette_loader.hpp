#pragma once

#include <cstdint>
#include <vector>
#include "bento/struct.hpp"

namespace nb
{
  class Palette_loader
  {
  public:
    std::vector<Palette> list;

    Palette_loader(const int InitialSize);
    void Unload();

    Palette *Add(const uint8_t pid);
    Palette *Get(const uint8_t pid);
    void Remove(const uint8_t pid);
  };
}