#include "../FileData.hpp"
#include "../Tracelog.hpp"
#include "../Assert.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace ppx
{
  FileData::FileData(const char *filename)
    : data(nullptr), length(0)
  {
    Load(filename);
  }

  bool FileData::Load(const char *filename)
  {
    if (isValid()) Unload();

    FILE *ptr_file = nullptr;
    uint8_t *temp_buffer = nullptr;
    size_t bytes_read;
    size_t file_size = 0;
    bool success = false;

    // Validate filename
    if (filename == nullptr || filename[0] == '\0') {
      TraceLog("FileData: empty filename");
      goto cleanup;
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
    
    file_size = ftell(ptr_file);
    if (file_size < 0) {
      TraceLog("FileData: ftell failed, %s", filename);
      goto cleanup;
    }
    
    rewind(ptr_file);

    // Validate file size
    if (file_size == 0) {
      TraceLog("FileData: empty file, %s", filename);
      goto cleanup;
    }

    // Allocate buffer
    temp_buffer = static_cast<uint8_t*>(malloc(file_size));
    if (!temp_buffer) {
      TraceLog("FileData: malloc failed, %s", filename);
      goto cleanup;
    }

    // Read file
    bytes_read = fread(temp_buffer, 1, file_size, ptr_file);
    if (bytes_read != file_size) {
      TraceLog("FileData: partially loaded %zu/%zu, %s", bytes_read, file_size, filename);
      goto cleanup;
    }

    // Success
    data = temp_buffer;
    length = file_size;
    success = true;
    TraceLog("FileData: load success, %s", filename);

cleanup:
    if (ptr_file) fclose(ptr_file);
    if (!success) {
      if (temp_buffer) free(temp_buffer);
      TraceLog("FileData: load failed, %s", filename);
    }
    return success;
  }

  void FileData::Unload()
  {
    if (data) free(data);
    data = nullptr;
    length = 0;
  }

  bool FileData::isValid() const
  {
    return (data != nullptr);
  }
}