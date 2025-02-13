#pragma once
#include <cstdio>

namespace ppx
{
  #ifndef NDEBUG
    #define TraceLog(fmt, ...) fprintf(stderr, fmt"\n", ##__VA_ARGS__)
  #else
    #define TraceLog(fmt, ...) ((void)0)
  #endif

}