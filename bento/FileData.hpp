#pragma once

#include <cstddef>
#include <cstdint>

namespace ppx {

  class FileData
  {
  public:
    uint8_t *data;
    size_t length;

    constexpr FileData() : data(nullptr), length(0) { }
    FileData(const char *filename);

    virtual int Load(const char *filename);
    virtual void Unload();
    virtual bool isValid();
  };

}