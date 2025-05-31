#pragma once

#include <cstddef>
#include <cstdint>

namespace ppx {

  class FileData
  {
  public:
    uint8_t *data = nullptr;
    size_t length = 0;

    constexpr FileData() : data(nullptr), length(0) { }
    FileData(const char *filename);

    bool Load(const char *filename);
    void Unload();
    bool isValid() const;
  };

}