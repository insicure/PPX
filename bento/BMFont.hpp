#include "bento/Texture.hpp"
#include <cstdint>

namespace ppx
{
  class BMFont
  {
  public:
    struct Char
    {
      uint32_t id = 0;
      uint16_t x = 0;
      uint16_t y = 0;
      uint16_t width = 0;
      uint16_t height = 0;
      int16_t xoffset = 0;
      int16_t yoffset = 0;
      int16_t xadvance = 0;
      uint8_t page = 0;
    };

    struct Kerning
    {
      uint32_t first = 0;
      uint32_t second = 0;
      int16_t amount = 0;
    };

    // Common block data
    uint16_t lineHeight;
    uint16_t base;
    uint16_t pages;

    Texture *pageTextures;

    // Glyphs and kerning
    uint32_t charCount;
    Char* chars;

    uint32_t kerningCount;
    Kerning* kernings;

    bool Load(const char* filename);
    void Unload();
    bool isValid() const;
  };
}