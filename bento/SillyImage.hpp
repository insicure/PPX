#pragma once

#include "FileData.hpp"
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
    ImageType_INDEXED_32_A3, // 5bpp, Bit5-7: Alpha
    ImageType_INDEXED_8_A5, // 3bpp, Bit3-7: Alpha
    ImageType_PALETTE_16,
    // ImageType_TILE_8x8,
    ImageType_INVALID,
  };

  #pragma pack(push, 1) 
  struct SillyImageMetadata {
    uint64_t header;
    uint8_t version;
    uint8_t format;
    uint8_t paletteId;
    uint16_t width;
    uint16_t height;
    uint8_t compression;
    uint32_t length;
  };
  #pragma pack(pop)

  class SillyImage : public FileData
  {
  public:
    uint16_t width;
    uint16_t height;
    uint8_t paletteId;
    ImageType format;

    constexpr SillyImage()
      : FileData(), width(0), height(0), paletteId(0), format(ImageType_INVALID), originaldata(nullptr) { }

    SillyImage(const char *filename);

    int Load(const char *filename) override;
    void Unload() override;
    bool isValid() override;

  private:
    unsigned char *originaldata;
  };
}