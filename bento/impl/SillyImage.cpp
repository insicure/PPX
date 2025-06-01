#include "../SillyImage.hpp"
#include "bento/Tracelog.hpp"
#include "fastlz.h"
#include <cstdio>
#include <cstdlib>

#define SILLYIMAGE_HEADER 7452728928599042419

namespace ppx
{
  bool SillyImage::Load(const char *filename)
  {
    if (isValid()) Unload();

    FILE *ptr_file = nullptr;
    uint8_t *temp_compress = nullptr;
    uint32_t original_size = 0;
    uint32_t compress_size = 0;
    bool success = false;

    uint64_t head = 0;

    // Validate filename
    if (!filename || filename[0] == '\0') {
      TraceLog("SillyImage: Empty filename");
      return false;
    }

    // Open file
    ptr_file = fopen(filename, "rb");
    if (!ptr_file) {
      TraceLog("SillyImage: fopen failed: %s", filename);
      goto cleanup;
    }

    // Verify header
    if (fread(&head, sizeof(head), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read header: %s", filename);
      goto cleanup;
    }

    if (head != SILLYIMAGE_HEADER) {
      TraceLog("SillyImage: Invalid header: %s", filename);
      goto cleanup;
    }

    // Verify version
    uint8_t version;
    if (fread(&version, sizeof(version), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read version: %s", filename);
      goto cleanup;
    }

    if (version != 13) {
      TraceLog("SillyImage: Unsupported version: %s", filename);
      goto cleanup;
    }

    // Verify format
    uint8_t raw_format;
    if (fread(&raw_format, sizeof(raw_format), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read format: %s", filename);
      goto cleanup;
    }
    
    if (raw_format > static_cast<uint8_t>(ImageType::ImageType_PALETTE_16)) {
      TraceLog("SillyImage: Invalid format: %s", filename);
      goto cleanup;
    }

    switch (raw_format) {
      case 0: format = ImageType_R8G8B8A8; break;
      case 1: format = ImageType_R5G5B5A1; break;
      case 2: format = ImageType_INDEXED_4; break;
      case 3: format = ImageType_INDEXED_16; break;
      case 4: format = ImageType_INDEXED_256; break;
      case 5: format = ImageType_INDEXED_32_A8; break;
      case 6: format = ImageType_INDEXED_8_A32; break;
      case 7: format = ImageType_PALETTE_16; break;
    }

    // Read dimensions
    if (fread(&width, sizeof(width), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read width: %s", filename);
      goto cleanup;
    }
    
    if (fread(&height, sizeof(height), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read height: %s", filename);
      goto cleanup;
    }

    // Read palette
    if (fread(&palette_count, sizeof(palette_count), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read palette_count: %s", filename);
      goto cleanup;
    }

    if (fread(&palette_hash, sizeof(palette_hash), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read palette_hash: %s", filename);
      goto cleanup;
    }

    // Read buffer size
    if (fread(&original_size, sizeof(original_size), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read original_size: %s", filename);
      goto cleanup;
    }

    if (fread(&compress_size, sizeof(compress_size), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read compress_size: %s", filename);
      goto cleanup;
    }

    // Allocate palette buffer
    palette_data = static_cast<uint8_t*>(malloc(palette_count * sizeof(uint16_t)));
    if (!palette_data) {
      TraceLog("SillyImage: malloc palette_data failed: %s", filename);
      goto cleanup;
    }

    if (fread(palette_data, sizeof(uint16_t), palette_count, ptr_file) != palette_count) {
      TraceLog("SillyImage: Failed read palette_data: %s", filename);
      goto cleanup;
    }

    // Allocate image buffers
    data = static_cast<uint8_t*>(malloc(original_size));
    if (!data) {
      TraceLog("SillyImage: malloc data failed: %s", filename);
      goto cleanup;
    }

    temp_compress = static_cast<uint8_t*>(malloc(compress_size));
    if (!temp_compress) {
      TraceLog("SillyImage: malloc temp_compress failed: %s", filename);
      goto cleanup;
    }

    // Decompress image data
    if (fread(temp_compress, 1, compress_size, ptr_file) != compress_size) {
      TraceLog("SillyImage: Failed read compressed data: %s", filename);
      goto cleanup;
    }

    if (!fastlz_decompress(temp_compress, compress_size, data, original_size)) {
      TraceLog("SillyImage: Decompression failed: %s", filename);
      goto cleanup;
    }

    success = true;
    TraceLog("SillyImage: Loaded successfully: %s", filename);

cleanup:
    if (temp_compress) free(temp_compress);
    if (ptr_file) fclose(ptr_file);

    if (!success) Unload();
    return success;
  }

  void SillyImage::Unload()
  {
    free(data);
    data = nullptr;

    free(palette_data);
    palette_data = nullptr;

    width = height = 0;
    palette_count = 0;
    palette_hash = 0;
    format = ImageType_INVALID;
  }

  bool SillyImage::isValid()
  {
    return (data != nullptr || format != ImageType_INVALID);
  }
}