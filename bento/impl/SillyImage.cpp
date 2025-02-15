#include "../SillyImage.hpp"
#include "../FileData.hpp"
#include "../Tracelog.hpp"
#include "../Assert.hpp"
#include <cstdlib>

#define SILLYIMAGE_HEADER 0x676D69796C6C6973

namespace ppx
{

  SillyImage::SillyImage(const char *filename)
    : data(nullptr), width(0), height(0), paletteid(0), format(ImageType_INVALID), originaldata()
  {
    Load(filename);
  }

  bool SillyImage::Load(const char *filename)
  {

    bool res = false;

    do
    {
      res = originaldata.Load(filename);
      if (!res) break;

      // check if metadata exist
      const bool cond = originaldata.length > sizeof(SillyImageMetadata);
      res_sassert(res, cond, "SillyImage: length < SillyImageMetadata, %s", filename);
      if (!res) break;

      SillyImageMetadata meta = *(SillyImageMetadata*)originaldata.data;

      // check header
      res_sassert(res, meta.header == SILLYIMAGE_HEADER, "SillyImage: invalid meta header, %s", filename);
      if (!res) break;

      // check version
      res_sassert(res, meta.version == 0, "SillyImage: invalid meta version, %s", filename);
      if (!res) break;

      // check format
      res_sassert(res, meta.format <= 7, "SillyImage: invalid meta format, %s", filename);
      switch (meta.format) {
        case 0: format = ImageType_R8G8B8A8; break;
        case 1: format = ImageType_R5G5B5A1; break;
        case 2: format = ImageType_INDEXED_4; break;
        case 3: format = ImageType_INDEXED_16; break;
        case 4: format = ImageType_INDEXED_256; break;
        case 5: format = ImageType_INDEXED_32_A3; break;
        case 6: format = ImageType_INDEXED_8_A5; break;
        case 7: format = ImageType_PALETTE_16; break;
        
        default: res = false; break;
      }
      if (!res) break;

      paletteid = meta.paletteid;
      width = meta.width;
      height = meta.height;
      data = (originaldata.data+sizeof(SillyImageMetadata));

    } while(0);

    if (!res)
    {
      TraceLog("SillyImage: load failed, %s", filename);
      Unload();
    }
    else
    {
      // TraceLog("SillyImage: load success, (%ux%u) pid:%u %s", width, height, paletteid, filename);
    }

    return res;
  }

  void SillyImage::Unload()
  {
    if (originaldata.isValid())
    {
      originaldata.Unload();
      // TraceLog("SillyImage: unloaded");
    }

    data = nullptr;
    format = ImageType_INVALID;
    width = 0;
    height = 0;
  }

  bool SillyImage::isValid()
  {
    return (data != nullptr || format != ImageType_INVALID);
  }
}