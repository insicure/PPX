#pragma once

#include <cstdint>

namespace ppx
{
  class Color {
  public:
    uint8_t a = 0;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

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

    constexpr inline uint32_t ToRGB255() const
    {
      return (static_cast<uint32_t>(a) << 24) |
             (static_cast<uint32_t>(r) << 16) |
             (static_cast<uint32_t>(g) << 8) |
             (static_cast<uint32_t>(b));
    }

    constexpr inline uint16_t ToRGB15(uint8_t alpha_threshold = 128) const
    {
      const uint16_t alpha_bit = (a >= alpha_threshold) ? 0b1000000000000000 : 0;
      return alpha_bit
          | (((uint8_t)r >> 3) & 0b00011111)
          | (((uint8_t)g >> 3) & 0b00011111) << 5
          | (((uint8_t)b >> 3) & 0b00011111) << 10;
    }

    static constexpr Color LightGray() { return Color(255, 200, 200, 200); }
    static constexpr Color Gray() { return Color(255, 128, 128, 128); }
    static constexpr Color DarkGray() { return Color(255, 80, 80, 80); }
    static constexpr Color Yellow() { return Color(255, 253, 249, 0); }
    static constexpr Color Gold() { return Color(255, 255, 203, 0); }
    static constexpr Color Orange() { return Color(255, 255, 161, 0); }
    static constexpr Color Pink() { return Color(255, 255, 109, 194); }
    static constexpr Color Red() { return Color(255, 230, 41, 55); }
    static constexpr Color Maroon() { return Color(255, 190, 33, 55); }
    static constexpr Color Green() { return Color(255, 0, 228, 48); }
    static constexpr Color Lime() { return Color(255, 0, 158, 47); }
    static constexpr Color DarkGreen() { return Color(255, 0, 117, 44); }
    static constexpr Color SkyBlue() { return Color(255, 102, 191, 255); }
    static constexpr Color Blue() { return Color(255, 0, 121, 241); }
    static constexpr Color DarkBlue() { return Color(255, 0, 82, 172); }
    static constexpr Color Purple() { return Color(255, 200, 122, 255); }
    static constexpr Color Violet() { return Color(255, 135, 60, 190); }
    static constexpr Color DarkPurple() { return Color(255, 112, 31, 126); }
    static constexpr Color Beige() { return Color(255, 211, 176, 131); }
    static constexpr Color Brown() { return Color(255, 127, 106, 79); }
    static constexpr Color DarkBrown() { return Color(255, 76, 63, 47); }

    static constexpr Color White() { return Color(255, 255, 255, 255); }
    static constexpr Color Black() { return Color(255, 0, 0, 0); }
    static constexpr Color Blank() { return Color(0, 0, 0, 0); }
    static constexpr Color Magenta() { return Color(255, 255, 0, 255); }
    static constexpr Color RayWhite() { return Color(255, 245, 245, 245); }
  };

  static inline bool operator==(const Color& lhs, const Color& rhs)
  {
    return (lhs.a == rhs.a) && (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b);
  }

  static inline bool operator!=(const Color& lhs, const Color& rhs)
  {
    return !(lhs == rhs);
  }
}