#pragma once

#include <cstdint>

namespace ppx {

  class FileData
  {
  public:
    static FileData* Load(const char *filename);
    void Unload();
    bool isValid() const;

    ~FileData() { Unload(); }
    uint8_t *data = nullptr;
    uint32_t length = 0;

  private:
    FileData() = default;
    FileData& operator=(const FileData&) = delete;
  };

}