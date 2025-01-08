#pragma once

#include "bento/color.hpp"
#include "bento/struct.hpp"
#include <cstdint>
#include <nds/ndstypes.h>

namespace nb
{

  #define TRANS_NONE    0x00
  #define TRANS_FLIP_X  0x01
  #define TRANS_FLIP_Y  0x02

// --- General Drawing --- 

  /**
   * @brief initialize 3d engine using gl2d
   */
  void Initialize();

  /**
   * @brief clear entire screen
   * 
   * @param color rgb255 color, the color will be converted into rgb16
   */
  void ClearBackground(const Color &color);

  /**
  * @brief call this function before drawing stuff,
  */
  void BeginDrawing();

  /**
  * @brief call this function after drawing stuff,
  */
  void EndDrawing();

  /**
   * @brief push 2d camera matrix
   * 
   * @param cam camera matrix
   */
  void BeginCamera(const Camera &cam);

  /**
  * @brief pop 2d camera matrix
  */
  void EndCamera();

  /**
   * @brief Set color tint
   * 
   * @param color rgb255 color, the color will be converted into rgb16
   */
  void SetColor(const Color &color);

  /**
  * @brief set alpha value f32 version
  * 
  * @param ratio value 0-1
  */
  void SetAlpha(const f32 &ratio);

  /**
  * @brief set alpha value int version
  * 
  * @param ratio value 0-31
  */
  void SetAlphai(const int value);

// --- Basic Shapes ---

  /**
   * @brief draw single pixel using gl_triangle
   * 
   * @param position x, y position on the screen
   */
  void DrawPixel(const Vec2 &position);

  /**
   * @brief draw line using gl_triangle
   * 
   * @param startPos start x, y position on the screen
   * @param endPos end x, y position on the screen
   */
  void DrawLine(const Vec2 &startPos, const Vec2 &endPos);

  /**
   * @brief draw line with thickness using gl_triangle_strip, the line thickness might not be accurate on some angle due to fixed point conversion
   * 
   * @param startPos start x, y position on the screen
   * @param endPos end x, y position on the screen
   * @param thick the thickness
   */
  void DrawLine(const Vec2 &startPos, const Vec2 &endPos, const f32 &thick);

  /**
   * @brief draw filled rectangle
   * 
   * @param rec rectangle to be drawn
   */
  void DrawRect(const Rect &rec);

  /**
   * @brief draw filled rectangle with rotation and origin
   * 
   * @param rec rectangle to be drawn
   * @param origin x, y origin position relative to rectangle position
   * @param rotation rotation degrees 0-360, will be converted into libnds angle
   */
  void DrawRect(const Rect &rec, const Vec2 &origin, const int16_t rotation);

  /**
  * @brief draw rectangle lines
  * 
  * @param rec rectangle to be drawn
  */
  void DrawRectLines(const Rect &rec);

  /**
  * @brief draw rectangle lines with thickness, the line thickness might not be accurate on some angle due to fixed point conversion
  * 
  * @param rec rectangle to be drawn
  * @param lineThick the thickness
  */
  void DrawRectLines(const Rect &rec, const f32 &lineThick);

  /**
   * @brief draw filled triangle, counter-clockwise order
   * 
   * @param v1 first point
   * @param v2 second point
   * @param v3 third point
   */
  void DrawTriangle(const Vec2 &v1, const Vec2 &v2, const Vec2 &v3);

  /**
   * @brief draw triangle lines, counter-clockwise order
   * 
   * @param v1 first point
   * @param v2 second point
   * @param v3 third point
   */
  void DrawTriangleLines(const Vec2 &v1, const Vec2 &v2, const Vec2 &v3);

  /**
   * @brief draw filled circle, the circle quality is low because it quite expensive to draw
   * 
   * @param position x, y position on the screen
   * @param radius circle radius
   */
  void DrawCircle(const Vec2 &position, f32 radius);

  /**
  * @brief draw circle line, the circle quality is low because it quite expensive to draw
  * 
  * @param position x, y position on the screen
  * @param radius circle radius
  */
  void DrawCircleLines(const Vec2 &position, f32 radius);

  /**
   * @brief draw a polygon
   * 
   * @param vertices point array
   * @param length the vertices array size
   */
  void DrawPolyLines(const Vec2 vertices[], const int16_t length);

  /**
  * @brief draw a polygon with thickness, the line thickness might not be accurate on some angle due to fixed point conversion
  * 
  * @param vertices point array
  * @param length the vertices array size
  */
  void DrawPolyLines(const Vec2 vertices[], const int16_t length, const f32 &thick);

// --- Texture Drawing ---

  void DrawTexture(const Texture &texture, const int transform, const Vec2 &position);
  void DrawTexture(const Texture &texture, const int transform, const Vec2 &position, const int rotation, const Vec2 &scale, const Vec2 origin);
  void DrawTexture(const Texture &texture, const Rect &region, const int transform, const Rect &dest, const int rotation, const Vec2 &origin);
  
  void DrawTextureTiled(const Texture &texture, const Rect &source, const Rect &dest, const Vec2 &origin, const int rotation, const f32 &scale);

  // CPU Image drawing function
  void ImageClearBlack(const Image &dst);
  void ImageClearWhite(const Image &dst);
  void ImageClear(const Image &dst, const Color &color);
  
  void ImageDrawHline(const Image &dst, const Vec2 &start, const Vec2 &end, const Color &color);
  void ImageDrawRect(const Image &dst, const Rect &rect, const Color &color);
  
  void ImageDraw(const Image &dst, const Image &image, const Vec2 &position);
  void ImageDraw(const Image &dst, const Image &image, const Vec2 &position, const Color &colorkey);
  void ImageDraw(const Image &dst, const Image &image, const Vec2 &position, const int colorkeyIndex);
  
  void ImageDraw(const Image &dst, const Image &image, const Rect &region, const Vec2 &position);
  void ImageDraw(const Image &dst, const Image &image, const Rect &region, const Vec2 &position, const Color &colorkey);
  void ImageDraw(const Image &dst, const Image &image, const Rect &region, const Vec2 &position, const int colorkeyIndex);

  // BitmapFont drawing function
  void DrawBMF(const BMF &bmf, const char* text, const Vec2 &position);
}