#include "bento/Color.hpp"
#include "bento/Texture.hpp"
#include <cstdint>

namespace ppx
{
  class BMFont
  {
  public:
    enum Alignment { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT };

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
    int16_t fontSize;
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

    const Char* GetChar(uint32_t id) const;
    int16_t GetKerning(uint32_t first, uint32_t second) const;

    uint32_t EncodeUTF8(const char *text, int &byteSize);
    void DrawString(const char* text, const Vec2 position, int max_width = 0, int max_height = 0, const Color tint = 0xffffff);
    void DrawGlyph(const Char* glyph, const Vec2 position, const Color tint = 0xffffff);
  };
}