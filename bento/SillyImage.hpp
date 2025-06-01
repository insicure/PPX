#pragma once

#include <cstdint>

namespace ppx
{
  enum ImageType
  {
    ImageType_R8G8B8A8 = 0,
    ImageType_R5G5B5A1,
    ImageType_INDEXED_4, // 2bpp
    ImageType_INDEXED_16, // 4bpp
    ImageType_INDEXED_256, // 8bpp
    ImageType_INDEXED_32_A8, // 5bpp, Bit5-7: Alpha
    ImageType_INDEXED_8_A32, // 3bpp, Bit3-7: Alpha
    ImageType_PALETTE_16,
    // ImageType_TILE_8x8,
    ImageType_INVALID,
  };

  class SillyImage
  {
  public:
    uint8_t *data = nullptr;
    uint16_t width = 0;
    uint16_t height = 0;
    uint16_t palette_count = 0;
    uint32_t palette_hash = 0;
    uint8_t *palette_data = nullptr;
    uint8_t format = ImageType_INVALID;

    bool Load(const char *filename);
    void Unload();
    bool isValid() const;

  private:
    uint8_t *temp_compress = nullptr;
  };
}