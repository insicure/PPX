#include "../SillyImage.hpp"
#include "../Memory.hpp"
#include "../Tracelog.hpp"
#include "fastlz.h"

#define SILLYIMAGE_HEADER 7452728928599042419
#define SILLYIMAGE_VERSION 14

namespace ppx
{
    SillyImage *Load_SillyImage(const char *filename)
  {
    SillyImage *ptr_result = nullptr;
    FILE *ptr_file = nullptr;
    uint8_t *temp_compress = nullptr;
    uint32_t original_size = 0;
    uint32_t compress_size = 0;
    bool success = false;

    ptr_result = ppx_malloc<SillyImage>();
    if (!ptr_result) return nullptr;
    
    // Validate filename
    if (!filename || filename[0] == '\0') {
      TraceLog("SillyImage: Empty filename");
      return nullptr;
    }
    
    // Open file
    ptr_file = fopen(filename, "rb");
    if (!ptr_file) {
      TraceLog("SillyImage: fopen failed: %s", filename);
      goto cleanup;
    }
    
    // Verify header
    uint64_t head;
    if (fread(&head, sizeof(head), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read header: %s", filename);
      goto cleanup;
    }

    if (head != SILLYIMAGE_HEADER) {
      TraceLog("SillyImage: Invalid header: %s", filename);
      goto cleanup;
    }

    // Verify version
    int8_t version;
    if (fread(&version, sizeof(version), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read version: %s", filename);
      goto cleanup;
    }

    if (version != SILLYIMAGE_VERSION) {
      TraceLog("SillyImage: Unsupported version: %s", filename);
      goto cleanup;
    }

    // Verify format
    int8_t raw_format;
    if (fread(&raw_format, sizeof(raw_format), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read format: %s", filename);
      goto cleanup;
    }
    
    if (raw_format > static_cast<uint8_t>(ImageType::ImageType_PALETTE_16)) {
      TraceLog("SillyImage: Invalid format: %s", filename);
      goto cleanup;
    }

    switch (raw_format) {
      case 0: ptr_result->format = ImageType_R8G8B8A8; break;
      case 1: ptr_result->format = ImageType_R5G5B5A1; break;
      case 2: ptr_result->format = ImageType_INDEXED_4; break;
      case 3: ptr_result->format = ImageType_INDEXED_16; break;
      case 4: ptr_result->format = ImageType_INDEXED_256; break;
      case 5: ptr_result->format = ImageType_INDEXED_32_A8; break;
      case 6: ptr_result->format = ImageType_INDEXED_8_A32; break;
      case 7: ptr_result->format = ImageType_PALETTE_16; break;
    }

    
    int8_t isBE;
    if (fread(&isBE, sizeof(isBE), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read big endian mode: %s", filename);
      goto cleanup;
    }
    
    if (isBE != 0)
    {
      TraceLog("SillyImage: big endian mode not supported: %s", filename);
      goto cleanup;
    }
    
    int8_t palette_format;
    if (fread(&palette_format, sizeof(palette_format), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read palette_format: %s", filename);
      goto cleanup;
    }

    if (palette_format != 1)
    {
      TraceLog("SillyImage: only rgb16 palette_format supported: %s", filename);
      goto cleanup;
    }

    // Read dimensions
    if (fread(&ptr_result->width, sizeof(ptr_result->width), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read width: %s", filename);
      goto cleanup;
    }
    
    if (fread(&ptr_result->height, sizeof(ptr_result->height), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read height: %s", filename);
      goto cleanup;
    }

    // Read palette
    if (fread(&ptr_result->palette_count, sizeof(ptr_result->palette_count), 1, ptr_file) != 1) {
      TraceLog("SillyImage: Failed read palette_count: %s", filename);
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
    ptr_result->palette_data = ppx_malloc<uint16_t>(ptr_result->palette_count);
    if (!ptr_result->palette_data) {
      TraceLog("SillyImage: alloc palette_data failed: %s", filename);
      goto cleanup;
    }

    if (fread(ptr_result->palette_data, sizeof(uint16_t), ptr_result->palette_count, ptr_file) != static_cast<size_t>(ptr_result->palette_count)) {
      TraceLog("SillyImage: Failed read palette_data: %s", filename);
      goto cleanup;
    }

    // Allocate image buffers
    ptr_result->data = ppx_malloc<uint8_t>(original_size);
    if (!ptr_result->data) {
      TraceLog("SillyImage: alloc data failed: %s", filename);
      goto cleanup;
    }

    temp_compress = ppx_malloc<uint8_t>(compress_size);
    if (!temp_compress) {
      TraceLog("SillyImage: alloc temp_compress failed: %s", filename);
      goto cleanup;
    }

    // Decompress image data
    if (fread(temp_compress, 1, compress_size, ptr_file) != compress_size) {
      TraceLog("SillyImage: Failed read compressed data: %s", filename);
      goto cleanup;
    }

    if (!fastlz_decompress(temp_compress, compress_size, ptr_result->data, original_size)) {
      TraceLog("SillyImage: Decompression failed: %s", filename);
      goto cleanup;
    }

    success = true;
    TraceLog("SillyImage: Loaded successfully: %s", filename);

cleanup:
    if (temp_compress) ppx_free(temp_compress);
    if (ptr_file) fclose(ptr_file);

    if (!success)
    {
      TraceLog("SillyImage: Failed to load: %s", filename);
      Unload_SillyImage(ptr_result);
    }

    return ptr_result;
  }

  void Unload_SillyImage(SillyImage *&ptr)
  {
    if (ptr)
    {
      ppx_free(ptr->data);
      ppx_free(ptr->palette_data);
      ppx_free(ptr);
    }
  }
}