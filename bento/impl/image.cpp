#include "bento/file.hpp"
#include "bento/struct.hpp"
#include "bento/utils.hpp"
#include <cstdlib>
namespace nb
{
  Image::Image(const char *filename)
    : data(nullptr), width(0), height(0), format(ImageType_INVALID)
  {
    Load(filename);
  }

  void Image::Unload()
  {
    free(originaldata);
    data = nullptr;
    format = ImageType_INVALID;
    width = 0;
    height = 0;
    originaldata = nullptr;
  }
  
  int Image::Load(const char *filename)
  {
    size_t size;
    originaldata = LoadFileData(filename, size);
    if (originaldata == nullptr)
    {
      TraceLog("img io: open failed %s", filename);
      return -1;
    }

    if (size < sizeof(SillyImageMetadata))
    {
      TraceLog("img io: img size less than SillyImageMetadata size, %s", filename);
      Unload();
      return -1;
    }

    SillyImageMetadata meta = *(SillyImageMetadata*)originaldata;

    if (meta.header != 0x676D69796C6C6973)
    {
      TraceLog("img io: invalid header metadata, %s", filename);
      Unload();
      return -1;
    }

    if (meta.version != 0)
    {
      TraceLog("img io: invalid version metadata, %s", filename);
      Unload();
      return -1;
    }

    switch (meta.format) {
      case 0: format = ImageType_R8G8B8A8; break;
      case 1: format = ImageType_R5G5B5A1; break;
      case 2: format = ImageType_INDEXED_4; break;
      case 3: format = ImageType_INDEXED_16; break;
      case 4: format = ImageType_INDEXED_256; break;
      case 5: format = ImageType_INDEXED_32_A3; break;
      case 6: format = ImageType_INDEXED_8_A5; break;
      case 7: format = ImageType_PALETTE_16; break;

      default:
      {
        TraceLog("img io: invalid format metadata, %s", filename);
        Unload();
        return -1;
      }
    }

    paletteId = meta.paletteId;
    width = meta.width;
    height = meta.height;
    data = (originaldata+sizeof(SillyImageMetadata));

    TraceLog("img io: loaded %s", filename);
    return 0;
  }

  bool Image::isValid()
  {
    return (this->data != nullptr || this->format != ImageType_INVALID);
  }
}