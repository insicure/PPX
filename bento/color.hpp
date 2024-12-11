#pragma once

#include <cstdint>
namespace nb
{
  class Color {
  public:
    uint8_t a, r, g, b;

    constexpr Color() : a(0), r(0), g(0), b(0) { }

    constexpr Color(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
    : a(a), r(r), g(g), b(b) { }

    constexpr Color(uint8_t r, uint8_t g, uint8_t b)
    : a(255), r(r), g(g), b(b) { }

    constexpr Color(uint32_t color)
    : a((color >> 24) & 0xFF), 
      r((color >> 16) & 0xFF),
      g((color >> 8) & 0xFF),
      b(color & 0xFF) { }

    constexpr inline uint32_t ARGB() 
    {
      return (static_cast<uint32_t>(a) << 24) |
             (static_cast<uint32_t>(r) << 16) |
             (static_cast<uint32_t>(g) << 8) |
             (static_cast<uint32_t>(b));
    }

    constexpr inline uint16_t ARGB16() 
    {
      return (static_cast<uint16_t>(a & 0x01) << 15) |
             (static_cast<uint16_t>(r >> 3)) |
             (static_cast<uint16_t>(g >> 3) << 5) |
             (static_cast<uint16_t>(b >> 3) << 10);
    }
  };

  constexpr Color LightGray (255, 200, 200, 200);
  constexpr Color Gray (255, 128, 128, 128);
  constexpr Color DarkGray (255, 80, 80, 80);
  constexpr Color Yellow (255, 253, 249, 0);
  constexpr Color Gold (255, 255, 203, 0);
  constexpr Color Orange (255, 255, 161, 0);
  constexpr Color Pink (255, 255, 109, 194);
  constexpr Color Red (255, 230, 41, 55);
  constexpr Color Maroon (255, 190, 33, 55);
  constexpr Color Green (255, 0, 228, 48);
  constexpr Color Lime (255, 0, 158, 47);
  constexpr Color DarkGreen (255, 0, 117, 44);
  constexpr Color SkyBlue (255, 102, 191, 255);
  constexpr Color Blue (255, 0, 121, 241);
  constexpr Color DarkBlue (255, 0, 82, 172);
  constexpr Color Purple (255, 200, 122, 255);
  constexpr Color Violet (255, 135, 60, 190);
  constexpr Color DarkPurple (255, 112, 31, 126);
  constexpr Color Beige (255, 211, 176, 131);
  constexpr Color Brown (255, 127, 106, 79);
  constexpr Color DarkBrown (255, 76, 63, 47);

  constexpr Color White (255, 255, 255, 255);
  constexpr Color Black (255, 0, 0, 0);
  constexpr Color Blank (0, 0, 0, 0);
  constexpr Color Magenta (255, 255, 0, 255);
  constexpr Color RayWhite (255, 245, 245, 245);
}