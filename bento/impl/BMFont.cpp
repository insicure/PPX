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
  char* new_path = (char*)malloc(base_len + name_len + strlen(".sillyimg") + 1);
  if (!new_path) return NULL;

  if (base_len) memcpy(new_path, filepath, base_len);
  memcpy(new_path + base_len, filename, name_len);
  strcpy(new_path + base_len + name_len, ".sillyimg");

  return new_path;
}

namespace ppx
{
  bool BMFont::Load(const char * filename)
  {
    FILE* file = fopen(filename, "rb");
    bool success = false;
    BlockHeader block_header;
    void* block_data = nullptr;
    char* pending_imgpath = nullptr;
    char* pending_basepath = nullptr;

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
          if (pending_imgpath) free(pending_imgpath);

          // Copy entire pages block to pending storage
          // handle later incase pages block come before common block
          pending_imgpath = static_cast<char*>(malloc(block_header.size));
          memcpy(pending_imgpath, block_data, block_header.size);
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
      block_data = nullptr;
    }

    // Process pending pages (must be done after Common block)
    if (pending_imgpath) {
      // Allocate memory for texture pointers
      pageTextures = new Texture[pages];

      char* ptr = pending_imgpath;

      for (uint16_t i=0; i<pages; i++) {
        size_t len = strlen(ptr);

        if (len == 0 || len >= 256)
        {
          TraceLog("BMFont: invalid page filename");
          goto cleanup;
        }

        pending_basepath = build_path_from_base(filename, ptr);

        if (!pageTextures[i].Load(pending_basepath))
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
    if (!success) Unload();

    if (file) fclose(file);
    if (block_data) free(block_data);
    if (pending_imgpath) free(pending_imgpath);
    if (pending_basepath) free(pending_basepath);

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