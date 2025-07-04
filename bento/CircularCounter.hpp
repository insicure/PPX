#pragma once

#include "Math.hpp"

namespace ppx
{
  class CircularCounter {
  public:
    int max = 0;
    int value = 0;

    CircularCounter(int max = 0) : max(max), value(0) {}

    inline int Next() {
      value = Math::mod(value+1, max+1);
      return value;
    }

    inline int Prev() {
      value = Math::mod(value+max, max+1);
      return value;
    } 
  };
}