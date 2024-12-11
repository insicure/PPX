#pragma once

#include "bento/struct.hpp"
#include <cstddef>
#include <nds/ndstypes.h>

namespace nb {

  unsigned char *LoadFileData(const char *filename, size_t &dataSize);
  const char *GetFileExtension(const char *filename);

  int LoadImage(Image &image, const char *filename);
  void UnloadImage(Image &image);

  int LoadTexture(Texture &texture, const char *filename);
  void UnloadTexture(Texture &texture);

  int LoadBMF(BMF &bmf, const char *filename);
  void UnloadBMF(BMF &bmf);
}