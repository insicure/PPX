#include "../FileData.hpp"
#include "../ErrorEnum.hpp"
#include "../Tracelog.hpp"
#include "nds/arm9/sassert.h"
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

  int FileData::Load(const char *filename)
  {
    if (isValid()) Unload();

    FILE *file = nullptr;
    int error = Error_OK;
    
    while (true)
    {
      // check filename
      if (filename == nullptr || strlen(filename) == 0)
      {
        error = Error_FileData_EmptyFilename;
        break;
      }

      // fopen
      file = fopen(filename, "rb");
      sassert(file != nullptr, "FileData: fopen failed");
      if (file == nullptr)
      {
        error = Error_FileData_fopenFailed;
        break;
      }

      // get length
      fseek(file, 0, SEEK_END);
      length = ftell(file);
      fseek(file, 0, SEEK_SET);

      sassert(length > 0, "FileData: length < 0");
      if (length < 0)
      {
        error = Error_FileData_InvalidLength;
        break;
      }

      // allocate memory
      data = (uint8_t*)malloc(length * sizeof(uint8_t));
      sassert(data != nullptr, "FileData: malloc failed");
      if (data == nullptr)
      {
        error = Error_FileData_AllocateFailed;
        break;
      }

      // read
      int count = fread(data, sizeof(uint8_t), length, file);
      sassert(count == length, "FileData: parially loaded");
      if (count != length)
      {
        error = Error_FileData_PartiallyLoaded;
        break;
      }

      break;
    }

    if (file) fclose(file);
    if (error != Error_OK) Unload();
    else TraceLog("FileData: loaded %s", filename);

    return error;
    fclose(file);
  }

  void FileData::Unload()
  {
    if (data) free(data);
    data = nullptr;
    length = 0;
  }

  bool FileData::isValid()
  {
    return (data != nullptr) && (length > 0);
  }
}