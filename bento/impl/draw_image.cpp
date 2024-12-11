#include "bento/struct.hpp"
#include "bento/drawing.hpp"
#include "nds/arm9/sassert.h"

namespace nb
{
  void ImageClearBlack(const Image &dst)
  {
    sassert(false, "currently not supported!");
  }

  void ImageClearWhite(const Image &dst)
  {
    sassert(false, "currently not supported!");
  }

  void ImageClear(const Image &dst, const Color &color)
  {
    sassert(false, "currently not supported!");
  }


  void ImageDrawHline(const Image &dst, const Vec2 &start, const Vec2 &end, const Color &color)
  {
    sassert(false, "currently not supported!");
  }

  void ImageDrawRect(const Image &dst, const Rect &rect, const Color &color)
  {
    sassert(false, "currently not supported!");
  }


  void ImageDraw(const Image &dst, const Image &image, const Vec2 &position)
  {
    sassert(false, "currently not supported!");
  }

  void ImageDraw(const Image &dst, const Image &image, const Vec2 &position, const Color &colorkey)
  {
    sassert(false, "currently not supported!");
  }

  void ImageDraw(const Image &dst, const Image &image, const Vec2 &position, const int colorkeyIndex)
  {
    sassert(false, "currently not supported!");
  }


  void ImageDraw(const Image &dst, const Image &image, const Rect &region, const Vec2 &position)
  {
    sassert(false, "currently not supported!");
  }

  void ImageDraw(const Image &dst, const Image &image, const Rect &region, const Vec2 &position, const Color &colorkey)
  {
    sassert(false, "currently not supported!");
  }

  void ImageDraw(const Image &dst, const Image &image, const Rect &region, const Vec2 &position, const int colorkeyIndex)
  {
    sassert(false, "currently not supported!");
  }
}