#include "../BMFont.hpp"
#include "bento/Texture.hpp"
#include "bento/Tracelog.hpp"
#include <cstdio>
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
#pragma pack(pop)

namespace ppx
{
  bool BMFont::Load(const char * filename)
  {
    FILE* file = fopen(filename, "rb");
    bool success = false;
    BlockHeader block_header;
    void* block_data = nullptr;
    char* pending = nullptr;
    uint32_t pending_size = 0;

    // Validate filename
    if (filename == nullptr || filename[0] == '\0')
    {
      TraceLog("BMFont: fopen failed, %s", filename);
      goto cleanup;
    }

    FileHeader header;
    if (fread(&header, sizeof(header), 1, file) != 1 || 
        memcmp(header.id, "BMF", 3) != 0 ||
        header.version != 3)
    {
      TraceLog("BMFont: invalid header, %s", filename);
      goto cleanup;
    }


    while (fread(&block_header, sizeof(block_header), 1, file) == 1) {
      block_data = malloc(block_header.size);
      if (!block_data) {
        TraceLog("BMFont: malloc failed for block %d", block_header.type);
        goto cleanup;
      }

      if (fread(block_data, block_header.size, 1, file) != 1) {
        TraceLog("BMFont: block read failed, block type %d", block_header.type);
        goto cleanup;
      }

      switch (block_header.type) {
        case 2:
        { // Common block
          auto *common = static_cast<CommonBlock*>(block_data);
          lineHeight = common->lineHeight;
          base = common->base;
          pages = common->pages;
          break;
        }

        case 3:
        { // Pages block
          // Free any previous pending data
          if (pending) free(pending);

          // Copy entire pages block to pending storage
          // handle later incase pages block come before common block
          pending = static_cast<char*>(malloc(block_header.size));
          memcpy(pending, block_data, block_header.size);
          pending_size = block_header.size;
          break;
        }

        case 4:
        { // Chars block
          charCount = block_header.size / sizeof(BMFont::Char);
          chars = static_cast<BMFont::Char*>(malloc(block_header.size));
          memcpy(chars, block_data, block_header.size);
          break;
        }

        case 5:
        { // Kerning pairs
          kerningCount = block_header.size / sizeof(BMFont::Kerning);
          kernings = static_cast<BMFont::Kerning*>(malloc(block_header.size));
          memcpy(kernings, block_data, block_header.size);
          break;
        }
      }

      free(block_data);
    }

    // Process pending pages (must be done after Common block)
    if (pending) {
      // Allocate memory for texture pointers
      pageTextures = new Texture[pages];

      char* ptr = pending;

      for (uint16_t i=0; i<pages; i++) {
        size_t len = strlen(ptr);

        if (len == 0 || len >= 256)
        {
          TraceLog("BMFont: invalid page filename");
          goto cleanup;
        }

        if (!pageTextures[i].Load(ptr))
        {
          TraceLog("BMFont: failed to load texture %s", ptr);
          goto cleanup;
        }

        ptr += len + 1;
      }
    }

    success = true;
    TraceLog("BMFont: loaded successfully, %s", filename);

cleanup:
    if (!success) Unload();

    if (file) fclose(file);
    if (block_data) free(block_data);
    if (pending) free(pending);

    return success;
  }

  void BMFont::Unload()
  {
    if (pageTextures) {
      delete[] pageTextures;
      pageTextures = nullptr;
    }
    if (chars) {
      free(chars);
      chars = nullptr;
    }
    if (kernings) {
      free(kernings);
      kernings = nullptr;
    }

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
}