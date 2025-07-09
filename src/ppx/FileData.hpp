#pragma once

#include <cstdint>

namespace ppx {

  struct FileData
  {
    uint8_t *data = nullptr;
    uint32_t length = 0;
  };

  FileData *Load_FileData(const char *filename);
  void Load_FileData(FileData *&ptr);
  void Unload_FileData(FileData *&ptr);
}