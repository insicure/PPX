#pragma once

#include "bento/f32.hpp"
#include <cstdint>
#include <cstdio>

namespace nb {

  class Vec2 {
  public:
    f32 x, y;

    constexpr Vec2()                 : x(), y() { }
    constexpr Vec2(f32 x, f32 y)     : x(x), y(y) { }
  };

  static inline Vec2 operator*(const Vec2 &lhs, const f32 &rhs)   { return Vec2(lhs.x * rhs, lhs.y * rhs); }
  static inline Vec2 operator/(const Vec2 &lhs, const f32 &rhs)   { return Vec2(lhs.x / rhs, lhs.y / rhs); }
  static inline Vec2 operator+(const Vec2 &lhs, const f32 &rhs)   { return Vec2(lhs.x + rhs, lhs.y + rhs); }
  static inline Vec2 operator-(const Vec2 &lhs, const f32 &rhs)   { return Vec2(lhs.x - rhs, lhs.y - rhs); }

  static inline Vec2 &operator*=(Vec2 &lhs, const f32 &rhs)       { lhs.x = lhs.x * rhs, lhs.y = lhs.y * rhs; return lhs; }
  static inline Vec2 &operator/=(Vec2 &lhs, const f32 &rhs)       { lhs.x = lhs.x / rhs, lhs.y = lhs.y / rhs; return lhs; }
  static inline Vec2 &operator+=(Vec2 &lhs, const f32 &rhs)       { lhs.x = lhs.x + rhs, lhs.y = lhs.y + rhs; return lhs; }
  static inline Vec2 &operator-=(Vec2 &lhs, const f32 &rhs)       { lhs.x = lhs.x - rhs, lhs.y = lhs.y - rhs; return lhs; }

  static inline bool operator==(const Vec2 &lhs, const Vec2 &rhs)    { return (lhs.x == rhs.x) && (lhs.y == rhs.y); }
  static inline bool operator!=(const Vec2 &lhs, const Vec2 &rhs)    { return (lhs.x != rhs.x) && (lhs.y != rhs.y); }

  class Rect {
  public:
    f32 x;
    f32 y;
    f32 width;
    f32 height;

    constexpr Rect()                           : x(), y(), width(), height() { }
    constexpr Rect(f32 x, f32 y, f32 w, f32 h) : x(x), y(y), width(w), height(h) { }
    constexpr Rect(f32 w, f32 h)               : x(), y(), width(w), height(h) { }

    inline f32 Left() const   { return x; }
    inline f32 Right() const  { return x+width; }
    inline f32 Top() const    { return y; }
    inline f32 Bottom() const { return y+height; }
  };

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

    constexpr static inline Matrix Identity() { return Matrix(1, 0, 0, 0, 1, 0, 0, 0, 1); }
    constexpr static inline Matrix Zero() { return Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0); }
    constexpr static inline Matrix One() { return Matrix(1, 1, 1, 1, 1, 1, 1, 1, 1); }
    constexpr static inline Matrix Translate(const f32 &x, const f32 &y) { return Matrix(1, 0, x, 0, 1, y, 0, 0, 1); }

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

  struct CircularCounter {
    int max;
    int value;

    CircularCounter(int max = 0) : max(max), value(0) {}

    inline int Next() {
      value = (value + 1) % (max + 1);
      return value;
    }

    inline int Prev() {
      value = (value - 1 + max + 1) % (max + 1);
      return value;
    } 
  };

  enum ImageType {
    ImageType_R8G8B8A8 = 0,
    ImageType_R5G5B5A1,
    ImageType_INDEXED_4, // 2bpp
    ImageType_INDEXED_16, // 4bpp
    ImageType_INDEXED_256, // 8bpp
    ImageType_INDEXED_32_A3, // 5bpp, Bit5-7: Alpha
    ImageType_INDEXED_8_A5, // 3bpp, Bit3-7: Alpha
    ImageType_PALETTE_16,
    // ImageType_TILE_8x8,
    ImageType_INVALID,
  };

  struct Camera {
    Vec2 offset;
    Vec2 target;
    int16_t rotation;
    f32 zoom;
  };

  class Image {
  public:
    unsigned char *data;
    uint16_t width;
    uint16_t height;
    uint8_t paletteId;
    ImageType format;

    constexpr Image()
      : data(nullptr), width(0), height(0), paletteId(0), format(ImageType_INVALID), originaldata(nullptr) { }

    Image(const char *filename);
    void Unload();

    int Load(const char *filename);
    bool isValid();

  private:
    unsigned char *originaldata;
  };

  class Texture {
  public:
    int id;
    uint16_t width;
    uint16_t height;

    constexpr Texture() : id(0), width(0), height(0) {}

    Texture(const Image &image);
    Texture(const char *filename);
    void Unload();

    int Load(const Image &image);
    int Load(const char *filename);
    bool isValid();
  };

  struct Palette {
    uint8_t pid;
    int texid;
  };

  #pragma pack(push, 1) 
  struct SillyImageMetadata {
    uint64_t header;
    uint8_t version;
    uint8_t format;
    uint8_t paletteId;
    uint16_t width;
    uint16_t height;
    uint8_t compression;
    uint32_t length;
  };
  #pragma pack(pop)

  struct BMFChar {
    uint8_t id;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    int16_t xoffset;
    int16_t yoffset;
    int16_t xadvance;
  };

  struct BMFKerning {
    uint8_t first;
    uint8_t second;
    int16_t amount;
  };

  struct BMF {
    uint8_t fontSize;
    uint8_t lineHeight;
    uint8_t base;
    BMFChar chars[96];
    BMFKerning *kernings;
    uint16_t kerningsLength;
    Texture texture;
  };

  struct TextureMap : public Texture
  {
    uint32_t hash;
    int16_t x;
    int16_t y;
    int16_t frame_x;
    int16_t frame_y;
    int16_t frame_width;
    int16_t frame_height;
    bool rotated;
  };

  class TextureAtlas
  {
  public:
    Texture **texture;
    TextureMap **map;

    int numTexture;
    int numMap;

    constexpr TextureAtlas() : texture(nullptr), map(nullptr), numTexture(0), numMap(0) {}
    TextureAtlas(const char* filename);
    ~TextureAtlas();

    int Load(const char* filename);
    TextureMap *Find(const char *name) const;
    void Unload();
    bool isValid();
  
  private:
    void readString(FILE *file, char *buffer);
  };

}