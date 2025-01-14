#include "../Tracelog.hpp"
#include <cstdarg>
#include <cstdio>

void ppx::TraceLog(const char *format, ...)
{
  #ifndef NDEBUG
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);

  // simply append a newline after printing
  fprintf(stderr, "\n");
  #endif
}