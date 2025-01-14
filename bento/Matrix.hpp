#pragma once

#include "Math.hpp"
#include "f32.hpp"

namespace ppx
{
  class Matrix {
  public:
    /* 
      | m00 m01 m02 |
      | m10 m11 m12 |
      | m20 m21 m22 |

    */
    f32 m00;
    f32 m10;
    f32 m20;

    f32 m01;
    f32 m11;
    f32 m21;

    f32 m02;
    f32 m12;
    f32 m22;

    constexpr Matrix() : m00(), m10(), m20(),
                         m01(), m11(), m21(),
                         m02(), m12(), m22() { }

    constexpr Matrix(f32 m00, f32 m01, f32 m02,
                     f32 m10, f32 m11, f32 m12,
                     f32 m20, f32 m21, f32 m22)
                    : m00(m00), m10(m10), m20(m20),
                      m01(m01), m11(m11), m21(m21),
                      m02(m02), m12(m12), m22(m22) { }

    constexpr static inline Matrix Zero() { return Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0); }
    constexpr static inline Matrix One()  { return Matrix(1, 1, 1, 1, 1, 1, 1, 1, 1); }
    
    constexpr static inline Matrix Identity()                               { return Matrix(1, 0, 0, 0, 1, 0, 0, 0, 1); }
    constexpr static inline Matrix Translation(const f32 &x, const f32 &y)  { return Matrix(1, 0, x, 0, 1, y, 0, 0, 1); }
    constexpr static inline Matrix Reflection(const bool x, const bool y)   { return Matrix((y) ? -1 : 1, 0, 0, 0, (x) ? -1 : 1, 0, 0, 0, 1); }
    constexpr static inline Matrix Scale(const f32 &x, const f32 &y)        { return Matrix(x, 0, 0, 0, y, 0, 0, 0, 1); }
    constexpr static inline Matrix Rotation(const int degree)               { return Matrix(math::cos(degree), -math::sin(degree), 0, math::sin(degree), math::cos(degree), 0, 0, 0, 1); }

    static inline f32 Determinate(const Matrix &m)
    {
      f32 v1 = m.m00 * (m.m22*m.m11 - m.m21*m.m12);
      f32 v2 = m.m10 * (m.m22*m.m01 - m.m21*m.m02);
      f32 v3 = m.m20 * (m.m12*m.m01 - m.m11*m.m02);
      return v1-v2+v3; 
    }

    static inline Matrix Inverse(const Matrix &m)
    {
      Matrix out;

      out.m00 = m.m22*m.m11 - m.m21*m.m12;
      out.m01 = m.m21*m.m02 - m.m22*m.m01;
      out.m02 = m.m12*m.m01 - m.m11*m.m02;
      out.m10 = m.m20*m.m12 - m.m22*m.m10;
      out.m11 = m.m22*m.m00 - m.m20*m.m02;
      out.m12 = m.m10*m.m02 - m.m12*m.m00;
      out.m20 = m.m21*m.m10 - m.m20*m.m11;
      out.m21 = m.m20*m.m01 - m.m21*m.m00;
      out.m22 = m.m11*m.m00 - m.m10*m.m01;

      return out * (1 / Determinate(m));
    }

    inline Matrix operator*(const f32 &rhs) const
    {
      return Matrix(
        m00*rhs, m01*rhs, m02*rhs,
        m10*rhs, m11*rhs, m12*rhs,
        m20*rhs, m21*rhs, m22*rhs
      );
    }

    inline Matrix operator/(const f32 &rhs) const
    {
      return Matrix(
        m00/rhs, m01/rhs, m02/rhs,
        m10/rhs, m11/rhs, m12/rhs,
        m20/rhs, m21/rhs, m22/rhs
      );
    }

    inline Matrix operator+(const f32 &rhs) const
    {
      return Matrix(
        m00+rhs, m01+rhs, m02+rhs,
        m10+rhs, m11+rhs, m12+rhs,
        m20+rhs, m21+rhs, m22+rhs
      );
    }

    inline Matrix operator-(const f32 &rhs) const
    {
      return Matrix(
        m00-rhs, m01-rhs, m02-rhs,
        m10-rhs, m11-rhs, m12-rhs,
        m20-rhs, m21-rhs, m22-rhs
      );
    }

    inline Matrix operator*(const Matrix &rhs) const
    {
      return Matrix(
        m00*rhs.m00 + m01*rhs.m10 + m02*rhs.m20,
        m00*rhs.m01 + m01*rhs.m11 + m02*rhs.m21,
        m00*rhs.m02 + m01*rhs.m12 + m02*rhs.m22,
        m10*rhs.m00 + m11*rhs.m10 + m12*rhs.m20,
        m10*rhs.m01 + m11*rhs.m11 + m12*rhs.m21,
        m10*rhs.m02 + m11*rhs.m12 + m12*rhs.m22,
        m20*rhs.m00 + m21*rhs.m10 + m22*rhs.m20,
        m20*rhs.m01 + m21*rhs.m11 + m22*rhs.m21,
        m20*rhs.m02 + m21*rhs.m12 + m22*rhs.m22
      );
    }

    inline Matrix operator+(const Matrix &rhs) const
    {
      return Matrix(
        m00+rhs.m00, m01+rhs.m01, m02+rhs.m02,
        m10+rhs.m10, m11+rhs.m11, m12+rhs.m12,
        m20+rhs.m20, m21+rhs.m21, m22+rhs.m22
      );
    }

    inline Matrix operator-(const Matrix &rhs) const
    {
      return Matrix(
        m00-rhs.m00, m01-rhs.m01, m02-rhs.m02,
        m10-rhs.m10, m11-rhs.m11, m12-rhs.m12,
        m20-rhs.m20, m21-rhs.m21, m22-rhs.m22
      );
    }

    inline Matrix &operator*=(const Matrix &rhs)
    {
      m00 = m00*rhs.m00 + m01*rhs.m10 + m02*rhs.m20;
      m01 = m00*rhs.m01 + m01*rhs.m11 + m02*rhs.m21;
      m02 = m00*rhs.m02 + m01*rhs.m12 + m02*rhs.m22;
      m10 = m10*rhs.m00 + m11*rhs.m10 + m12*rhs.m20;
      m11 = m10*rhs.m01 + m11*rhs.m11 + m12*rhs.m21;
      m12 = m10*rhs.m02 + m11*rhs.m12 + m12*rhs.m22;
      m20 = m20*rhs.m00 + m21*rhs.m10 + m22*rhs.m20;
      m21 = m20*rhs.m01 + m21*rhs.m11 + m22*rhs.m21;
      m22 = m20*rhs.m02 + m21*rhs.m12 + m22*rhs.m22;
      return *this;
    }

    inline Matrix &operator+=(const Matrix &rhs)
    {
      m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02;
      m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12;
      m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22;
      return *this;
    }

    inline Matrix &operator-=(const Matrix &rhs)
    {
      m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02;
      m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12;
      m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22;
      return *this;
    }

    inline bool operator==(const Matrix &rhs)
    {
      return (m00 == rhs.m00) && (m01 == rhs.m01) && (m02 == rhs.m02) &&
             (m10 == rhs.m10) && (m11 == rhs.m11) && (m12 == rhs.m12) &&
             (m20 == rhs.m20) && (m21 == rhs.m21) && (m22 == rhs.m22);
    }

    inline bool operator!=(const Matrix &rhs)
    {
      return (m00 != rhs.m00) && (m01 != rhs.m01) && (m02 != rhs.m02) &&
             (m10 != rhs.m10) && (m11 != rhs.m11) && (m12 != rhs.m12) &&
             (m20 != rhs.m20) && (m21 != rhs.m21) && (m22 != rhs.m22);
    }
  };
}