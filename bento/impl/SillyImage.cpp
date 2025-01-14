#include "../SillyImage.hpp"
#include "../ErrorEnum.hpp"
#include "../FileData.hpp"
#include "../Tracelog.hpp"
#include "nds/arm9/sassert.h"
#include <cstdlib>

#define SILLYIMAGE_HEADER 0x676D69796C6C6973

namespace ppx
{

  SillyImage::SillyImage(const char *filename)
    : FileData(), width(0), height(0), paletteId(0), format(ImageType_INVALID), originaldata(nullptr)
  {
    Load(filename);
  }

  int SillyImage::Load(const char *filename)
  {
    int error = FileData::Load(filename);
    if (error != Error_OK)
      return error;

    originaldata = data;
    data = nullptr;

    while(true)
    {
      sassert(length > sizeof(SillyImageMetadata), "SillyImage: length < SillyImageMetadata");
      if (length < sizeof(SillyImageMetadata))
      {
        error = Error_SillyImage_LengthMetadata;
        break;
      }

      SillyImageMetadata meta = *(SillyImageMetadata*)originaldata;
      
      sassert(meta.header == SILLYIMAGE_HEADER, "SillyImage: invalid header");
      if (meta.header != SILLYIMAGE_HEADER)
      {
        error = Error_SillyImage_InvalidHeader;
        break;
      }

      sassert(meta.version == 0, "SillyImage: invalid version");
      if (meta.version != 0)
      {
        error = Error_SillyImage_InvalidVersion;
        break;
      }

      sassert(meta.format <= 7, "SillyImage: invalid format");
      switch (meta.format) {
        case 0: format = ImageType_R8G8B8A8; break;
        case 1: format = ImageType_R5G5B5A1; break;
        case 2: format = ImageType_INDEXED_4; break;
        case 3: format = ImageType_INDEXED_16; break;
        case 4: format = ImageType_INDEXED_256; break;
        case 5: format = ImageType_INDEXED_32_A3; break;
        case 6: format = ImageType_INDEXED_8_A5; break;
        case 7: format = ImageType_PALETTE_16; break;

        default: error = Error_SillyImage_InvalidFormat; break;
      }
      if (error == Error_SillyImage_InvalidFormat) break;

      paletteId = meta.paletteId;
      width = meta.width;
      height = meta.height;
      data = (originaldata+sizeof(SillyImageMetadata));

      break;
    }

    if (error != Error_OK) Unload();
    else TraceLog("SillyImage: loaded %s", filename);

    return error;
  }

  void SillyImage::Unload()
  {
    if (originaldata) free(originaldata);

    data = nullptr;
    originaldata = nullptr;
    format = ImageType_INVALID;
    width = 0;
    height = 0;
  }

  bool SillyImage::isValid()
  {
    return (data != nullptr || format != ImageType_INVALID);
  }
}