#pragma once

#include <cstdint>
#include <new>

namespace ppx {

  template <typename T> inline T *ppx_alloc() noexcept {
    return new(std::nothrow) T;
  }

  template <typename T> inline T *ppx_alloc(uint32_t count) noexcept {
    return new(std::nothrow) T[count];
  }

  template <typename T> inline void ppx_free_array(T *&ptr) noexcept {
    delete[] ptr;
    ptr = nullptr;
  }

  template <typename T> inline void ppx_free_object(T *&ptr) noexcept {
    delete ptr;
    ptr = nullptr;
  }

}