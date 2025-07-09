#include "nds/arm9/sassert.h"
#include <cstdio>

namespace ppx
{
  #define res_sassert(res, cond, fmt, ...) \
    do { \
      res = cond; \
      sassert(cond, fmt, ##__VA_ARGS__); \
      if (!res) fprintf(stderr, fmt"\n", ##__VA_ARGS__); \
    } while(0)
}


