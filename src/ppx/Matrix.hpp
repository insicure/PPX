#pragma once

#include "Math.hpp"
#include "f32.hpp"

namespace ppx
{
  class Matrix {
  public:
    /* 
      [ m0, m1, m2 ]
      [ m3, m4, m5 ]
      [ m6, m7, m8 ]
    */
    f32 m[9] = {};

    const f32& operator[](int index) const { return m[index]; }
    f32& operator[](int index) { return m[index]; }

    constexpr Matrix() = default;

    constexpr static Matrix Zero()     { return {0, 0, 0, 0, 0, 0, 0, 0, 0}; }
    constexpr static Matrix One()      { return {1, 1, 1, 1, 1, 1, 1, 1, 1}; }
    constexpr static Matrix Identity() { return {1, 0, 0, 0, 1, 0, 0, 0, 1}; }
    
    static Matrix Translation(f32 x, f32 y) { return {1, 0, x, 0, 1, y, 0, 0, 1}; }
    static Matrix Scale(f32 x, f32 y)       { return {x, 0, 0, 0, y, 0, 0, 0, 1}; }
    static  Matrix Rotation(int degree)
    {
      f32 cos = Math::cos(degree);
      f32 sin = Math::cos(degree);
      return {cos, -sin, 0, sin, cos, 0, 0, 0, 1};
    }

    static Matrix Inverse(const Matrix &m)
    {
      f32 det = m[0]*(m[4]*m[8] - m[5]*m[7]) -
                m[1]*(m[3]*m[8] - m[5]*m[6]) +
                m[2]*(m[3]*m[7] - m[4]*m[6]);

      
      constexpr int epsilon = 1 << 6;
      if (Math::abs(det).value <= epsilon) return Matrix::Identity();
                
      f32 inv_det = (1 / det);

      return {
        inv_det * (m[4]*m[8] - m[5]*m[7]), 
        inv_det * (m[2]*m[7] - m[1]*m[8]),
        inv_det * (m[1]*m[5] - m[2]*m[4]),

        inv_det * (m[5]*m[6] - m[3]*m[8]),
        inv_det * (m[0]*m[8] - m[2]*m[6]),
        inv_det * (m[2]*m[3] - m[0]*m[5]),

        inv_det * (m[3]*m[7] - m[4]*m[6]),
        inv_det * (m[1]*m[6] - m[0]*m[7]),
        inv_det * (m[0]*m[4] - m[1]*m[3])
      };
    }

    Matrix operator*(const Matrix& rhs) const {
      return {
        m[0]*rhs[0] + m[1]*rhs[3] + m[2]*rhs[6],
        m[0]*rhs[1] + m[1]*rhs[4] + m[2]*rhs[7],
        m[0]*rhs[2] + m[1]*rhs[5] + m[2]*rhs[8],

        m[3]*rhs[0] + m[4]*rhs[3] + m[5]*rhs[6],
        m[3]*rhs[1] + m[4]*rhs[4] + m[5]*rhs[7],
        m[3]*rhs[2] + m[4]*rhs[5] + m[5]*rhs[8],

        m[6]*rhs[0] + m[7]*rhs[3] + m[8]*rhs[6],
        m[6]*rhs[1] + m[7]*rhs[4] + m[8]*rhs[7],
        m[6]*rhs[2] + m[7]*rhs[5] + m[8]*rhs[8]
      };
    }

    // ---

    Matrix &operator+=(const Matrix &rhs) {
      for (int i=0; i<9; i++) m[i] += rhs.m[i];
      return *this;
    }

    Matrix &operator-=(const Matrix &rhs) {
      for (int i=0; i<9; i++) m[i] -= rhs.m[i];
      return *this;
    }

    Matrix &operator*=(const Matrix &rhs) {
      *this = *this * rhs;
      return *this;
    }

    Matrix &operator*=(f32 scalar) {
      for (int i=0; i<9; i++) m[i] *= scalar;
      return *this;
    }

    Matrix &operator/=(f32 scalar) {
      for (int i=0; i<9; i++) m[i] /= scalar;
      return *this;
    }

    // ---

    Matrix operator+(const Matrix &rhs) const {
      Matrix result = *this;
      return result += rhs;
    }

    Matrix operator-(const Matrix &rhs) const {
      Matrix result = *this;
      return result -= rhs;
    }

    Matrix operator*(f32 scalar) const {
      Matrix result = *this;
      return result *= scalar;
    }

    Matrix operator/(f32 scalar) const {
      Matrix result = *this;
      return result /= scalar;
    }

    bool operator==(const Matrix &rhs)
    {
      for (int i=0; i<9; i++)
        if (m[i] != rhs[i]) return false;
      return true;
    }

    bool operator!=(const Matrix &rhs)
    {
      return !(*this == rhs);
    }
  };
}