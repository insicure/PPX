#pragma once
#include <cstdio>
#include <utility>

namespace ppx
{

#ifndef NDEBUG
  template<typename... Args>
  void TraceLog(const char* fmt, Args&&... args) {
    fprintf(stderr, fmt, std::forward<Args>(args)...);
    fprintf(stderr, "\n");
  }
#else
  template<typename... Args>
  void TraceLog(const char*, Args&&...) {}
#endif

}