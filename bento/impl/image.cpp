#include "bento/file.hpp"
#include "bento/struct.hpp"
#include "bento/utils.hpp"
#include <cstdlib>
namespace nb
{
  Image::Image(const char *filename, const int width, const int height, const ImageType format)
    : data(nullptr), width(width), height(height), format(format)
  {
    Load(filename, width, height, format);
  }

  Image::~Image()
  {
    free(this->data);
    this->data = nullptr;
    this->format = ImageType::INVALID;
    this->width = 0;
    this->height = 0;
    this->width_padding = 0;
  }
  
  int Image::Load(const char *filename, const int width, const int height, const ImageType format)
  {
    size_t size;
    this->data = LoadFileData(filename, size);
    if (data == nullptr)
    {
      TraceLog("img io: open failed %s", filename);
      return -1;
    }

    this->format = format;

    if (this->format == ImageType::UNCOMPRESSED_PALETTE16)
    {
      // use width/height as palette length
      this->width = size/2;
      this->height = size/2;
    }
    else
    {
      this->width = width;
      this->height = height;
    }

    TraceLog("img io: loaded %s", filename);
    return 0;
  }

  bool Image::isValid()
  {
    return (this->data != nullptr || this->format != ImageType::INVALID);
  }
}