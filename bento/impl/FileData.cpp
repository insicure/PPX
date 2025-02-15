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

    FILE *file = nullptr;
    bool res = false;

    do
    {

      // check filename
      const bool cond = (filename != nullptr || strlen(filename) != 0);
      res_sassert(res, cond, "FileData: empty filename");
      if (!res) break;

      // fopen
      file = fopen(filename, "rb");
      res_sassert(res, file != nullptr, "FileData: fopen failed, %s", filename);
      if (!res) break;

      // get length
      fseek(file, 0, SEEK_END);
      length = ftell(file);
      fseek(file, 0, SEEK_SET);

      // check length
      res_sassert(res, length > 0, "FileData: length < 0, %s", filename);
      if (!res) break;

      // allocate memory
      data = (uint8_t*)malloc(length * sizeof(uint8_t));
      res_sassert(res, data != nullptr, "FileData: malloc failed, %s", filename);
      if (!res) break;

      // read
      size_t count = fread(data, sizeof(uint8_t), length, file);
      res_sassert(res, count == length, "FileData: partially loaded, %s", filename);
      if (!res) break;

    } while(0);

    if (file) fclose(file);
    if (!res)
    {
      TraceLog("FileData: load failed, %s", filename);
      Unload();
    }
    else TraceLog("FileData: load success, %s", filename);

    return res;
    fclose(file);
  }

  void FileData::Unload()
  {
    if (isValid()) 
    {
      free(data);
      // TraceLog("FileData: unloaded");
    }

    data = nullptr;
    length = 0;
  }

  bool FileData::isValid()
  {
    return (data != nullptr);
  }
}