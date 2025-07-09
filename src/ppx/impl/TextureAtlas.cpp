#include "../TextureAtlas.hpp"
#include "../Assert.hpp"
#include "../external/murmurhash.h"
#include "../Tracelog.hpp"
#include "../Memory.hpp"
#include "../Texture.hpp"
#include "../TextureMap.hpp"
#include <cstring>


#define ATLAS_HEADER 0x68637263

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
    char* new_path = ppx_malloc<char>(base_len + name_len + strlen(".sillyimg") + 1);
    if (!new_path) return nullptr;

    if (base_len) memcpy(new_path, filepath, base_len);
    memcpy(new_path + base_len, filename, name_len);
    strcpy(new_path + base_len + name_len, ".sillyimg");

    return new_path;
  }

  TextureAtlas *Load_TextureAtlas(const char* filename)
  {
    TextureAtlas *ptr_result = nullptr;
    FILE *ptr_file = nullptr;
    bool success = false;
    char *str_texturepath = nullptr;

    ptr_result = ppx_malloc<TextureAtlas>();
    if (!ptr_result) return nullptr;

    ptr_result->pages = 0;
    ptr_result->pageTexture = nullptr;

    ptr_file = fopen(filename, "rb");
    if (!ptr_file) {
      TraceLog("FileData: fopen failed, %s", filename);
      goto cleanup;
    }

    char line[128];

    // check header
    {
      if (!fgets(line, sizeof(line), ptr_file))
      {
        TraceLog("TextureAtlas: invalid header, %s", filename);
        goto cleanup;
      }

      int count = sscanf(line, "sillyatl %hhu", &ptr_result->pages);
      if (count != 1)
      {
        TraceLog("TextureAtlas: invalid header, %s", filename);
        goto cleanup;
      }
    }

    ptr_result->pageTexture = ppx_calloc<TextureAtlas::TexturePages>(ptr_result->pages);
    if (!ptr_result->pageTexture)
    {
      TraceLog("TextureAtlas: allocate TextureAtlasItem failed, %s", filename);
      goto cleanup;
    }

    for (int i_tex=0; i_tex<ptr_result->pages; i_tex++)
    {
      TextureAtlas::TexturePages &item = ptr_result->pageTexture[i_tex];
      
      char texture_name[64];
      
      // get line
      if (!fgets(line, sizeof(line), ptr_file))
      {
        TraceLog("TextureAtlas: invalid format, %s", filename);
        goto cleanup;
      }

      int count = sscanf(line, "%s %hu", texture_name, &item.length);
      if (count != 2)
      {
        TraceLog("TextureAtlas: invalid format, %s", filename);
        goto cleanup;
      }

      str_texturepath = build_path_from_base(filename, texture_name);
      if (!str_texturepath)
      {
        TraceLog("TextureAtlas: failed to alloc texture path");
        goto cleanup;
      }

      item.texture = Load_Texture(str_texturepath);
      if (!item.texture)
      {
        TraceLog("TextureAtlas: failed to load texture, %s", str_texturepath);
        goto cleanup;
      }

      ppx_free(str_texturepath);

      item.images = ppx_malloc<TextureMap>(item.length);
      if (!item.images)
      {
        TraceLog("TextureAtlas: allocate TextureMap failed, %s", filename);
        goto cleanup;
      }

      // get images
      for (int i_img=0; i_img<item.length; i_img++)
      {
        TextureMap &image = item.images[i_img];

        char image_name[64];
        uint16_t temp_rotated = 0;

        if (!fgets(line, sizeof(line), ptr_file))
        {
          TraceLog("TextureAtlas: invalid format, %s", filename);
          goto cleanup;
        }

        int count = sscanf(line, "%s %hu %hu %hu %hu %hu %hu %hu %hu %hu",
                            image_name,
                            &image.frame_x,
                            &image.frame_y,
                            &image.frame_width,
                            &image.frame_height,
                            &image.offset_x,
                            &image.offset_y,
                            &image.width,
                            &image.height,
                            &temp_rotated);

        // TraceLog("%s %hu %hu %hu %hu %hu %hu %hu %hu %hu",
        //   image_name,
        //   image.frame_x,
        //   image.frame_y,
        //   image.frame_width,
        //   image.frame_height,
        //   image.offset_x,
        //   image.offset_y,
        //   image.width,
        //   image.height,
        //   temp_rotated);

        if (count != 10)
        {
          TraceLog("TextureAtlas: invalid format, %s", filename);
          goto cleanup;
        }

        image.rotated = (temp_rotated == 1);
        image.id = item.texture->id;
        image.hash = murmurhash(image_name, strlen(image_name), 0);
      }
    }

    success = true;
    TraceLog("TextureAtlas: loaded, %s", filename);

cleanup:
    if (ptr_file) fclose(ptr_file);
    if (str_texturepath) ppx_free(str_texturepath);

    if (!success)
    {
      TraceLog("TextureAtlas: load failed,  %s", filename);
      Unload_TextureAtlas(ptr_result);
    }

    return ptr_result;
  }

  void Unload_TextureAtlas(TextureAtlas *&ptr)
  {
    if (ptr)
    {
      if (ptr->pageTexture)
      {
        for (int i_tex=0; i_tex<ptr->pages; i_tex++)
        {
          if (ptr->pageTexture[i_tex].texture)
            Unload_Texture(ptr->pageTexture[i_tex].texture);

          if (ptr->pageTexture[i_tex].images)
            ppx_free(ptr->pageTexture[i_tex].images);
        }
      }

      ppx_free(ptr);
    }
  }

  TextureMap *TextureAtlas::operator[](const char *name)
  {
    return find(name);
  }

  TextureMap *TextureAtlas::find(const char *name)
  {
    uint32_t hash = murmurhash(name, strlen(name), 0);

    for (int i_tex=0; i_tex<pages; i_tex++)
    {
      TexturePages &item = pageTexture[i_tex];

      for (int i_img=0; i_img<item.length; i_img++)
      {
        if (hash == item.images[i_img].hash)
          return &item.images[i_img];
      }
    }
    return nullptr;
  }
}