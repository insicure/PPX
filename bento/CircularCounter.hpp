#pragma once

#include "Math.hpp"
#include <cstdint>

namespace ppx
{
  class CircularCounter {
  public:
    uint32_t max = 0;
    uint32_t value = 0;

    CircularCounter(uint32_t max = 0) : max(max), value(0) {}

    inline int Next() {
      value = math::mod(value+1, max+1);
      return value;
    }

    inline int Prev() {
      value = math::mod(value+max, max+1);
      return value;
    } 
  };
}