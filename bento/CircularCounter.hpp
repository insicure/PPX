#pragma once

#include "Math.hpp"

namespace ppx
{
  class CircularCounter {
  public:
    int max;
    int value;

    CircularCounter(int max = 0) : max(max), value(0) {}

    inline int Next() {
      value = math::mod(value+1, max+1);
      return value;
    }

    inline int Prev() {
      value = math::mod(value-1 + max+1, max+1);
      return value;
    } 
  };
}