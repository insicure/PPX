/*

#define STB_IMAGE_IMPLEMENTATION

// #define STBI_NO_JPEG
// #define STBI_NO_PNG
// #define STBI_NO_BMP
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM 
#include "external/stb_image.h"

*/

#include <nds/arm9/sassert.h>
#include <nds/arm9/videoGL.h>
#include <sys/_intsup.h>
#include "file.hpp"
#include "utils.hpp"
#include <cstddef>
#include <cstdio>

namespace nb {

  unsigned char *LoadFileData(const char *filename, size_t &dataSize)
  {
    unsigned char *data = nullptr;
    dataSize = 0;

    if (filename == nullptr)
      return nullptr;

    FILE *file = fopen(filename, "rb");
    if (file == nullptr)
    {
      TraceLog("file io: failed to opening file: %s", filename);
      return nullptr;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size <= 0)
    {
      fclose(file);
      TraceLog("file io: opening empty file: %s", filename);
      return nullptr;
    }

    data = (unsigned char *)malloc(size*sizeof(unsigned char));
    if (data == nullptr)
    {
      fclose(file);
      TraceLog("file io: opening file not enough memory: %s", filename);
    }

    int count = fread(data, sizeof(unsigned char), size, file);
    dataSize = count;

    if (dataSize != size)
      TraceLog("file io: file partialy loaded: %s", filename);
    else
      TraceLog("file io: file loaded: %s", filename);

    fclose(file);
    return data;
  }

  const char *GetFileExtension(const char *filename)
  {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return nullptr;
    return dot;
  }
}