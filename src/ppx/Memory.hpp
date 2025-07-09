#pragma once

#include <cstddef>
#include <cstdlib>
#include <type_traits>

namespace ppx {

  template <typename T>
  T* ppx_malloc(size_t count = 1) {
    static_assert(std::is_trivially_copyable_v<T>,
                 "Type must be trivially copyable");
    return static_cast<T*>(malloc(sizeof(T) * count));
  }

  template <typename T>
  T* ppx_calloc(size_t count = 1) {
    static_assert(std::is_trivially_copyable_v<T>,
                 "Type must be trivially copyable");
    return static_cast<T*>(calloc(count, sizeof(T)));
  }

  template <typename T>
  T* ppx_realloc(T* ptr, size_t new_count) {
    static_assert(std::is_trivially_copyable_v<T>,
                 "Type must be trivially copyable");
    return static_cast<T*>(realloc(static_cast<void*>(ptr),
                                      sizeof(T) * new_count));
  }

  template <typename T>
  T* ppx_aligned_alloc(size_t alignment, size_t count = 1) {
    static_assert(std::is_trivially_copyable_v<T>,
                 "Type must be trivially copyable");

    return static_cast<T*>(aligned_alloc(alignment, sizeof(T) * count));
  }

  template <typename T>
  inline void ppx_free(T*& ptr) {
    free(static_cast<void*>(ptr));
    ptr = nullptr;
  }

}