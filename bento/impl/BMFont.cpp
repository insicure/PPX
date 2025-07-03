#include "../BMFont.hpp"
#include "bento/Memory.hpp"
#include "bento/Texture.hpp"
#include "bento/Tracelog.hpp"
#include "bento/Vec2.hpp"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>

static_assert(true);
#pragma pack(push, 1)
struct FileHeader {
  char id[3];
  uint8_t version;
};

struct BlockHeader {
  uint8_t type;
  uint32_t size;
};

struct CommonBlock {
  uint16_t lineHeight;
  uint16_t base;
  uint16_t scaleW, scaleH;
  uint16_t pages;
  uint8_t packed;
  uint8_t alphaChnl, redChnl, greenChnl, blueChnl;
};

struct InfoBlock {
  int16_t fontSize;
  uint8_t bitField;
  uint8_t charSet;
  uint16_t stretchH;
  uint8_t aa;
  uint8_t paddingUp;
  uint8_t paddingRight;
  uint8_t paddingDown;
  uint8_t paddingLeft;
  uint8_t spacingHoriz;
  uint8_t spacingVert;
  uint8_t outline;
  char fontName[1]; // Variable length, minimum 1 byte
};
#pragma pack(pop)

namespace ppx
{
  static char* build_path_from_base(const char* filepath, const char* filename) {
    // Get base directory from filepath
    const char* last_slash = strrchr(filepath, '/');
    if (!last_slash) last_slash = strrchr(filepath, '\\');
    size_t base_len = last_slash ? (last_slash - filepath + 1) : 0;

    // Find filename extension
    const char* ext = strrchr(filename, '.');
    const char* last_fn_slash = strrchr(filename, '/');
    if (!last_fn_slash) last_fn_slash = strrchr(filename, '\\');

    size_t name_len;
    if (ext && (!last_fn_slash || ext > last_fn_slash)) {
        name_len = ext - filename; // Length without extension
    } else {
        name_len = strlen(filename); // No extension found
    }

    // Allocate and build new path
    char* new_path = ppx_alloc<char>(base_len + name_len + strlen(".sillyimg") + 1);
    if (!new_path) return NULL;

    if (base_len) memcpy(new_path, filepath, base_len);
    memcpy(new_path + base_len, filename, name_len);
    strcpy(new_path + base_len + name_len, ".sillyimg");

    return new_path;
  }
  
  BMFont *BMFont::Load(const char * filename)
  {
    BMFont *ptr_result = nullptr;
    FILE* ptr_file = fopen(filename, "rb");
    BlockHeader block_header;
    uint8_t* block_data = nullptr;
    char* pending_imgpath = nullptr;
    char* pending_basepath = nullptr;
    bool success = false;

    ptr_result = ppx_alloc<BMFont>();
    if (!ptr_result) return nullptr;

    // Validate filename
    if (filename == nullptr || filename[0] == '\0')
    {
      TraceLog("BMFont: fopen failed, %s", filename);
      goto cleanup;
    }

    FileHeader header;
    if (fread(&header, sizeof(header), 1, ptr_file) != 1 || 
        memcmp(header.id, "BMF", 3) != 0 ||
        header.version != 3)
    {
      TraceLog("BMFont: invalid header, %s", filename);
      goto cleanup;
    }


    while (fread(&block_header, sizeof(block_header), 1, ptr_file) == 1) {
      block_data = ppx_alloc<uint8_t>(block_header.size);
      if (!block_data) {
        TraceLog("BMFont: alloc failed for block %d", block_header.type);
        goto cleanup;
      }

      if (fread(block_data, block_header.size, 1, ptr_file) != 1) {
        TraceLog("BMFont: block read failed, block type %d", block_header.type);
        goto cleanup;
      }

      switch (block_header.type) {
        case 1: // Info block
        {
          auto* info = reinterpret_cast<InfoBlock*>(block_data);
          ptr_result->fontSize = std::abs(info->fontSize);
          break;
        }

        case 2:
        { // Common block
          auto *common = reinterpret_cast<CommonBlock*>(block_data);
          ptr_result->lineHeight = common->lineHeight;
          ptr_result->base = common->base;
          ptr_result->pages = common->pages;
          break;
        }

        case 3:
        { // Pages block
          // Free any previous pending data
          if (pending_imgpath) ppx_free_array(pending_imgpath);

          // Copy entire pages block to pending storage
          // handle later incase pages block come before common block
          pending_imgpath = ppx_alloc<char>(block_header.size);
          memcpy(pending_imgpath, block_data, block_header.size);
          break;
        }

        case 4:
        { // Chars block
          ptr_result->charCount = block_header.size / sizeof(BMFChar);
          ptr_result->chars = ppx_alloc<BMFChar>(block_header.size);
          memcpy(ptr_result->chars, block_data, block_header.size);
          break;
        }

        case 5:
        { // Kerning pairs
          ptr_result->kerningCount = block_header.size / sizeof(BMFKerning);
          ptr_result->kernings = ppx_alloc<BMFKerning>(block_header.size);
          memcpy(ptr_result->kernings, block_data, block_header.size);
          break;
        }
      }

      ppx_free_array(block_data);
    }

    // Process pending pages (must be done after Common block)
    if (pending_imgpath) {
      // Allocate memory for texture pointers
      ptr_result->pageTextures = ppx_alloc<Texture>(ptr_result->pages);

      char* ptr = pending_imgpath;

      for (uint16_t i=0; i<ptr_result->pages; i++) {
        size_t len = strlen(ptr);

        if (len == 0 || len >= 256)
        {
          TraceLog("BMFont: invalid page filename");
          goto cleanup;
        }

        // TODO: handle null
        pending_basepath = build_path_from_base(filename, ptr);

        if (!ptr_result->pageTextures[i].Load(pending_basepath))
        {
          TraceLog("BMFont: failed to load texture %s", pending_basepath);
          goto cleanup;
        }

        free(pending_basepath);
        pending_basepath = nullptr;
        
        ptr += len + 1;
      }
    }

    success = true;
    TraceLog("BMFont: loaded successfully, %s", filename);

cleanup:
    if (!success) {
      ptr_result->Unload();
      ppx_free_object(ptr_result);
      TraceLog("BMFont: load failed, %s", filename);
    }

    if (ptr_file) fclose(ptr_file);
    if (block_data) ppx_free_array(block_data);
    if (pending_imgpath) ppx_free_array(pending_imgpath);
    if (pending_basepath) ppx_free_array(pending_basepath);

    return ptr_result;
  }

  void BMFont::Unload()
  {
    if (pageTextures) ppx_free_array(pageTextures);
    if (chars) ppx_free_array(chars);
    if (kernings) ppx_free_array(kernings);

    lineHeight = 0;
    base = 0;
    pages = 0;
    charCount = 0;
    kerningCount = 0;
  }

  bool BMFont::isValid() const
  {
    return pageTextures != nullptr && 
           chars != nullptr && 
           pages > 0;
  }

  const BMFont::BMFChar* BMFont::GetChar(uint32_t id) const {
    if (!chars || charCount == 0) return nullptr;

    const BMFChar* first = chars;
    const BMFChar* last = chars + charCount;
    const BMFChar* it = std::lower_bound(first, last, id,
      [](const BMFChar& ch, uint32_t val) { return ch.id < val; });

    return (it != last && it->id == id) ? it : nullptr;
  }

  int16_t BMFont::GetKerning(uint32_t first_char, uint32_t second_char) const {
    if (!kernings || kerningCount == 0) return 0;

    // Kerning keys for search
    struct KerningKey {
      uint32_t first, second;
      KerningKey(uint32_t f, uint32_t s) : first(f), second(s) {}
    } key = {first_char, second_char};

    const BMFKerning* first = kernings;
    const BMFKerning* last = kernings + kerningCount;
    const BMFKerning* it = std::lower_bound(first, last, key,
      [](const BMFKerning& k, const KerningKey& key) {
        if (k.first != key.first) return k.first < key.first;
        return k.second < key.second;
      });

    return (it != last && it->first == first_char && it->second == second_char) ? 
            it->amount : 0;
  }

  uint32_t BMFont::EncodeUTF8(const char *text, int &byteSize)
  {
    uint8_t b0 = text[0];
    
    if ((b0 & 0b10000000) == 0) {  // 1-byte
      byteSize = 1;
      return b0;
    }

    if ((b0 & 0b11100000) == 0b11000000) {  // 2-byte
      uint8_t b1 = text[1];
      byteSize = 2;
      return ((b0 & 0b00011111) << 6) | (b1 & 0b00111111);
    }

    if ((b0 & 0b11110000) == 0b11100000) {  // 3-byte
      uint8_t b1 = text[1];
      uint8_t b2 = text[2];
      byteSize = 3;
      return ((b0 & 0b00001111) << 12) | ((b1 & 0b00111111) << 6) | (b2 & 0b00111111);
    }

    if ((b0 & 0b11111000) == 0b11110000) {  // 4-byte
      uint8_t b1 = text[1];
      uint8_t b2 = text[2];
      uint8_t b3 = text[3];
      byteSize = 4;
      return ((b0 & 0b00000111) << 18) | ((b1 & 0b00111111) << 12) | ((b2 & 0b00111111) << 6) | (b3 & 0b00111111);
    }

    byteSize = 0;
    return 0x3f;
  }

  void BMFont::DrawGlyph(const BMFChar *glyph, const Vec2 position, const Color tint)
  {
    pageTextures[glyph->page].Draw(
      {position.x+glyph->xoffset, position.y+glyph->yoffset},
      {1, 1}, {0, 0}, 0, false, false,
      {glyph->x, glyph->y, glyph->width, glyph->height},
      tint
    );
  }

  void BMFont::DrawStringEx(const char *text, const Vec2 position, int max_width, int max_height, const Color tint)
  {
    // Modified from the raylib [text] example - Rectangle bounds
    // https://github.com/raysan5/raylib/blob/bdda18656b301303b711785db48ac311655bb3d9/examples/text/text_rectangle_bounds.c#L143

    int length = strlen(text);

    int textOffsetY = 0;
    int textOffsetX = 0;

    enum { MEASURE_STATE = 0, DRAW_STATE = 1 };

    int state = (max_width>0) ? MEASURE_STATE : DRAW_STATE;

    int startLine = -1;
    int endLine = -1;
    int lastk = -1;

    for (int i=0, k=0; i<length; i++, k++)
    {
      // Get next codepoint from byte string and glyph index in font
      int codepointByteCount = 0;
      uint32_t codepoint = EncodeUTF8(&text[i], codepointByteCount);

      const BMFChar* glyph = GetChar(codepoint);
      if (!glyph) glyph = GetChar(0xFFFD);
      if (!glyph) glyph = GetChar('?');
      if (!glyph) glyph = GetChar(' ');

      // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
      // but we need to draw all of the bad bytes using the '?' symbol moving one byte
      if (codepoint == 0x3f) codepointByteCount = 1;
      i += (codepointByteCount - 1);

      int glyphWidth = 0;
      if (codepoint != '\n')
      {
        glyphWidth = (glyph->xadvance == 0) ? glyph->width : glyph->xadvance;
        // if (i + 1 < length) glyphWidth = glyphWidth + spacing;
      }

      // NOTE: When wordWrap is ON we first measure how much of the text we can draw before going outside of the rec container
      // We store this info in startLine and endLine, then we change states, draw the text between those two variables
      // and change states again and again recursively until the end of the text (or until we get outside of the container).
      // When wordWrap is OFF we don't need the measure state so we go to the drawing state immediately
      // and begin drawing on the next line before we can get outside the container.
      if (state == MEASURE_STATE)
      {
        // TODO: There are multiple types of spaces in UNICODE, maybe it's a good idea to add support for more
        // Ref: http://jkorpela.fi/chars/spaces.html
        if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n')) endLine = i;

        if ((textOffsetX + glyphWidth) > max_width)
        {
          endLine = (endLine < 1)? i : endLine;
          if (i == endLine) endLine -= codepointByteCount;
          if ((startLine + codepointByteCount) == endLine) endLine = (i - codepointByteCount);

          state = !state;
        }
        else if ((i + 1) == length)
        {
          endLine = i;
          state = !state;
        }
        else if (codepoint == '\n') state = !state;

        if (state == DRAW_STATE)
        {
          textOffsetX = 0;
          i = startLine;
          glyphWidth = 0;

          // Save character position when we switch states
          int tmp = lastk;
          lastk = k - 1;
          k = tmp;
        }
      }
      else // DRAW_STATE
      {
        if (codepoint == '\n')
        {
          if (!max_width)
          {
            textOffsetY += lineHeight;
            textOffsetX = 0;
          }
        }
        else
        {
          // When text overflows rectangle height limit, just stop drawing
          if (max_height && (textOffsetY + base) > max_height) break;

          // Draw current character glyph
          if ((codepoint != ' ') && (codepoint != '\t'))
          {
            DrawGlyph(glyph, {position.x + textOffsetX, position.y + textOffsetY}, tint);
          }
        }

        if (/* wordWrap */true && (i == endLine))
        {
          textOffsetY += lineHeight;
          textOffsetX = 0;
          startLine = endLine;
          endLine = -1;
          glyphWidth = 0;
          k = lastk;
          state = !state;
        }
      }

      if ((textOffsetX != 0) || (codepoint != ' ')) textOffsetX += glyphWidth;  // avoid leading spaces
    }

  }

}