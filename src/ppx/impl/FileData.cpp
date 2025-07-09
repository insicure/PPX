#include "../FileData.hpp"
#include "../Tracelog.hpp"
#include "../Memory.hpp"

namespace ppx
{
  FileData *Load_FileData(const char *filename)
  {
    FileData *ptr_result = nullptr;
    FILE *ptr_file = nullptr;
    uint32_t bytes_read;
    bool success = false;

    ptr_result = ppx_malloc<FileData>();
    if (!ptr_result) return nullptr;

    ptr_result->length = 0;
    ptr_result->data = nullptr;

    // Validate filename
    if (filename == nullptr || filename[0] == '\0') {
      TraceLog("FileData: empty filename");
      return nullptr;
    }
      
    // Open file
    ptr_file = fopen(filename, "rb");
    if (!ptr_file) {
      TraceLog("FileData: fopen failed, %s", filename);
      goto cleanup;
    }

    // Get file size
    if (fseek(ptr_file, 0, SEEK_END) != 0) {
      TraceLog("FileData: fseek(END) failed, %s", filename);
      goto cleanup;
    }
    
    ptr_result->length = ftell(ptr_file);
    if (ptr_result->length < 0) {
      TraceLog("FileData: ftell failed, %s", filename);
      goto cleanup;
    }
    
    rewind(ptr_file);

    // Validate file size
    if (ptr_result->length == 0) {
      TraceLog("FileData: empty file, %s", filename);
      goto cleanup;
    }

    // Allocate buffer
    ptr_result->data = ppx_malloc<uint8_t>(ptr_result->length);
    if (!ptr_result->data) {
      TraceLog("FileData: alloc failed, %s", filename);
      goto cleanup;
    }

    // Read file
    bytes_read = fread(ptr_result->data, 1, ptr_result->length, ptr_file);
    if (bytes_read != ptr_result->length) {
      TraceLog("FileData: partially loaded %zu/%zu, %s", bytes_read, ptr_result->length, filename);
      goto cleanup;
    }

    // Success
    success = true;
    TraceLog("FileData: load success, %s", filename);

cleanup:
    if (ptr_file) fclose(ptr_file);

    if (!success)
    {
      Unload_FileData(ptr_result);
      TraceLog("FileData: load failed, %s", filename);
    }
    return ptr_result;
  }

  void Unload_FileData(FileData *&ptr)
  {
    if (ptr)
    {
      if (ptr->data) ppx_free(ptr->data);
      ppx_free(ptr);
    }
  }
}