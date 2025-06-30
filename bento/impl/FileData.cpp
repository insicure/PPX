#include "../FileData.hpp"
#include "../Tracelog.hpp"
#include "../Memory.hpp"

namespace ppx
{
  FileData* FileData::Load(const char *filename)
  {
    FileData *ptr_result = nullptr;
    FILE *ptr_file = nullptr;
    uint8_t *temp_buffer = nullptr;
    uint32_t bytes_read;
    uint32_t file_size = 0;

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
    temp_buffer = ppx_alloc<uint8_t>(file_size);
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
    ptr_result = new(std::nothrow) FileData();
    if (ptr_result)
    {
      ptr_result->data = temp_buffer;
      ptr_result->length = file_size;
      TraceLog("FileData: load success, %s", filename);
    }

cleanup:
    if (ptr_file) fclose(ptr_file);
    if (!ptr_result && temp_buffer) {
      ppx_free_array(temp_buffer);
      TraceLog("FileData: load failed, %s", filename);
    }
    return ptr_result;
  }

  void FileData::Unload()
  {
    if (data) ppx_free_array(data);
    data = nullptr;
    length = 0;
  }

  bool FileData::isValid() const
  {
    return (data != nullptr);
  }
}