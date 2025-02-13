#pragma once

#include <nds/arm9/math.h>
#include <nds/arm9/trig_lut.h>

namespace ppx {
  class f32 {
  public:
    int value = 0;

    constexpr f32()              : value(0) { }
    constexpr f32(int value)     : value(intToFixed(value, 12)) { }
    constexpr f32(float value)   : value(floatToFixed(value, 12)) { }

    static inline f32 Raw(int value)
    {
      f32 out;
      out.value = value;
      return out;
    }

    constexpr inline int Get() const { return value; }
    constexpr inline int toInt() const { return fixedToInt(value, 12); }
    constexpr inline float toFloat() const { return fixedToFloat(value, 12); }

    inline f32 &operator=(const int rhs) { value = intToFixed(rhs, 12); return *this; }
    inline f32 &operator=(const float rhs) { value = floatToFixed(rhs, 12); return *this; }
    inline f32 &operator=(const f32 &rhs) { value = rhs.value; return *this; }
  };

  // f32 lhs rhs
  static inline f32 operator*(const f32 &lhs, const f32 &rhs)      { return f32::Raw(mulf32(lhs.value, rhs.value)); }
  static inline f32 operator/(const f32 &lhs, const f32 &rhs)      { return f32::Raw(divf32(lhs.value, rhs.value)); }
  static inline f32 operator+(const f32 &lhs, const f32 &rhs)      { return f32::Raw(lhs.value + rhs.value); }
  static inline f32 operator-(const f32 &lhs, const f32 &rhs)      { return f32::Raw(lhs.value - rhs.value); }

  static inline f32 &operator*=(f32 &lhs, const f32 &rhs)          { lhs.value = mulf32(lhs.value, rhs.value); return lhs; }
  static inline f32 &operator/=(f32 &lhs, const f32 &rhs)          { lhs.value = divf32(lhs.value, rhs.value); return lhs; }
  static inline f32 &operator+=(f32 &lhs, const f32 &rhs)          { lhs.value += rhs.value; return lhs; }
  static inline f32 &operator-=(f32 &lhs, const f32 &rhs)          { lhs.value -= rhs.value; return lhs; }

  static inline bool operator==(const f32 &lhs, const f32 &rhs)    { return lhs.value == rhs.value; }
  static inline bool operator!=(const f32 &lhs, const f32 &rhs)    { return lhs.value != rhs.value; }
  static inline bool operator>(const f32 &lhs, const f32 &rhs)     { return lhs.value > rhs.value; }
  static inline bool operator>=(const f32 &lhs, const f32 &rhs)    { return lhs.value >= rhs.value; }
  static inline bool operator<(const f32 &lhs, const f32 &rhs)     { return lhs.value < rhs.value; }
  static inline bool operator<=(const f32 &lhs, const f32 &rhs)    { return lhs.value <= rhs.value; }

  static inline f32 operator-(const f32 &lhs)                      { return f32::Raw(-lhs.value); }
  // static inline f32 operator--(f32 &lhs)                           { lhs -= 1; return lhs; }
  // static inline f32 operator++(f32 &lhs)                           { lhs += 1; return lhs; }

  // int rhs
  static inline f32 operator*(const f32 &lhs, const int rhs)   { return f32::Raw(mulf32(lhs.value, intToFixed(rhs, 12))); }
  static inline f32 operator/(const f32 &lhs, const int rhs)   { return f32::Raw(divf32(lhs.value, intToFixed(rhs, 12))); }
  static inline f32 operator+(const f32 &lhs, const int rhs)   { return f32::Raw(lhs.value + intToFixed(rhs, 12)); }
  static inline f32 operator-(const f32 &lhs, const int rhs)   { return f32::Raw(lhs.value - intToFixed(rhs, 12)); }

  static inline f32 &operator*=(f32 &lhs, const int rhs)       { lhs.value = mulf32(lhs.value, intToFixed(rhs, 12)); return lhs; }
  static inline f32 &operator/=(f32 &lhs, const int rhs)       { lhs.value = divf32(lhs.value, intToFixed(rhs, 12)); return lhs; }
  static inline f32 &operator+=(f32 &lhs, const int rhs)       { lhs.value += intToFixed(rhs, 12); return lhs; }
  static inline f32 &operator-=(f32 &lhs, const int rhs)       { lhs.value -= intToFixed(rhs, 12); return lhs; }

  static inline bool operator==(const f32 &lhs, const int rhs) { return lhs.value == intToFixed(rhs, 12); }
  static inline bool operator!=(const f32 &lhs, const int rhs) { return lhs.value != intToFixed(rhs, 12); }
  static inline bool operator>(const f32 &lhs, const int rhs)  { return lhs.value > intToFixed(rhs, 12); }
  static inline bool operator>=(const f32 &lhs, const int rhs) { return lhs.value >= intToFixed(rhs, 12); }
  static inline bool operator<(const f32 &lhs, const int rhs)  { return lhs.value < intToFixed(rhs, 12); }
  static inline bool operator<=(const f32 &lhs, const int rhs) { return lhs.value <= intToFixed(rhs, 12); }

  // int lhs
  static inline f32 operator*(const int lhs, const f32 &rhs)   { return f32::Raw(mulf32(intToFixed(lhs, 12), rhs.value)); }
  static inline f32 operator/(const int lhs, const f32 &rhs)   { return f32::Raw(divf32(intToFixed(lhs, 12), rhs.value)); }
  static inline f32 operator+(const int lhs, const f32 &rhs)   { return f32::Raw(intToFixed(lhs, 12) + rhs.value); }
  static inline f32 operator-(const int lhs, const f32 &rhs)   { return f32::Raw(intToFixed(lhs, 12) - rhs.value); }

  static inline int operator*=(int &lhs, const f32 &rhs)   { return lhs *= rhs.toInt(); }
  static inline int operator/=(int &lhs, const f32 &rhs)   { return lhs /= rhs.toInt(); }
  static inline int operator+=(int &lhs, const f32 &rhs)   { return lhs += rhs.toInt(); }
  static inline int operator-=(int &lhs, const f32 &rhs)   { return lhs -= rhs.toInt(); }

  static inline bool operator==(const int lhs, const f32 &rhs) { return intToFixed(lhs, 12) == rhs.value; }
  static inline bool operator!=(const int lhs, const f32 &rhs) { return intToFixed(lhs, 12) != rhs.value; }
  static inline bool operator>(const int lhs, const f32 &rhs)  { return intToFixed(lhs, 12) > rhs.value; }
  static inline bool operator>=(const int lhs, const f32 &rhs) { return intToFixed(lhs, 12) >= rhs.value; }
  static inline bool operator<(const int lhs, const f32 &rhs)  { return intToFixed(lhs, 12) < rhs.value; }
  static inline bool operator<=(const int lhs, const f32 &rhs) { return intToFixed(lhs, 12) <= rhs.value; }

  // float rhs
  static inline f32 operator*(const f32 &lhs, const float rhs)     { return f32::Raw(mulf32(lhs.value, floatToFixed(rhs, 12))); }
  static inline f32 operator/(const f32 &lhs, const float rhs)     { return f32::Raw(divf32(lhs.value, floatToFixed(rhs, 12))); }
  static inline f32 operator+(const f32 &lhs, const float rhs)     { return f32::Raw(lhs.value + floatToFixed(rhs, 12)); }
  static inline f32 operator-(const f32 &lhs, const float rhs)     { return f32::Raw(lhs.value - floatToFixed(rhs, 12)); }

  static inline f32 &operator*=(f32 &lhs, const float rhs)         { lhs.value = mulf32(lhs.value, floatToFixed(rhs, 12)); return lhs; }
  static inline f32 &operator/=(f32 &lhs, const float rhs)         { lhs.value = divf32(lhs.value, floatToFixed(rhs, 12)); return lhs; }
  static inline f32 &operator+=(f32 &lhs, const float rhs)         { lhs.value += floatToFixed(rhs, 12); return lhs; }
  static inline f32 &operator-=(f32 &lhs, const float rhs)         { lhs.value -= floatToFixed(rhs, 12); return lhs; }

  static inline bool operator==(const f32 &lhs, const float rhs)   { return lhs.value == floatToFixed(rhs, 12); }
  static inline bool operator!=(const f32 &lhs, const float rhs)   { return lhs.value != floatToFixed(rhs, 12); }
  static inline bool operator>(const f32 &lhs, const float rhs)    { return lhs.value > floatToFixed(rhs, 12); }
  static inline bool operator>=(const f32 &lhs, const float rhs)   { return lhs.value >= floatToFixed(rhs, 12); }
  static inline bool operator<(const f32 &lhs, const float rhs)    { return lhs.value < floatToFixed(rhs, 12); }
  static inline bool operator<=(const f32 &lhs, const float rhs)   { return lhs.value <= floatToFixed(rhs, 12); }

  // float lhs
  static inline f32 operator*(const float lhs, const f32 &rhs)     { return f32::Raw(mulf32(floatToFixed(lhs, 12), rhs.value)); }
  static inline f32 operator/(const float lhs, const f32 &rhs)     { return f32::Raw(divf32(floatToFixed(lhs, 12), rhs.value)); }
  static inline f32 operator+(const float lhs, const f32 &rhs)     { return f32::Raw(floatToFixed(lhs, 12) + rhs.value); }
  static inline f32 operator-(const float lhs, const f32 &rhs)     { return f32::Raw(floatToFixed(lhs, 12) - rhs.value); }

  static inline float operator*=(float &lhs, const f32 &rhs)       { return lhs *= rhs.toFloat(); }
  static inline float operator/=(float &lhs, const f32 &rhs)       { return lhs /= rhs.toFloat(); }
  static inline float operator+=(float &lhs, const f32 &rhs)       { return lhs += rhs.toFloat(); }
  static inline float operator-=(float &lhs, const f32 &rhs)       { return lhs -= rhs.toFloat(); }

  static inline bool operator==(const float lhs, const f32 &rhs)   { return floatToFixed(lhs, 12) == rhs.value; }
  static inline bool operator!=(const float lhs, const f32 &rhs)   { return floatToFixed(lhs, 12) != rhs.value; }
  static inline bool operator>(const float lhs, const f32 &rhs)    { return floatToFixed(lhs, 12) > rhs.value; }
  static inline bool operator>=(const float lhs, const f32 &rhs)   { return floatToFixed(lhs, 12) >= rhs.value; }
  static inline bool operator<(const float lhs, const f32 &rhs)    { return floatToFixed(lhs, 12) < rhs.value; }
  static inline bool operator<=(const float lhs, const f32 &rhs)   { return floatToFixed(lhs, 12) <= rhs.value; }

}