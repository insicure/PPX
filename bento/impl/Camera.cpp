#include "../Camera.hpp"
#include "../Math.hpp"

namespace ppx
{
  Vec2 Camera::CameraToScreen(const Vec2 &position)
  {
    Matrix translate(
      1, 0, position.x,
      0, 1, position.y,
      0, 0, 1);

    Matrix m = translate * GetMatrix();
    return Vec2(m.m02, m.m12);
  }

  Vec2 Camera::ScreenToScreen(const Vec2 &position)
  {
    Matrix translate(
      1, 0, position.x,
      0, 1, position.y,
      0, 0, 1);

    Matrix m = translate * Matrix::Inverse(GetMatrix());
    return Vec2(m.m02, m.m12);
  }

  Matrix Camera::GetMatrix()
  {
    Matrix m_origin(
      1, 0, -target.x,
      0, 1, -target.y,
      0, 0, 1);

    Matrix m_rotation(
      math::cos(rotation), -math::sin(rotation), 0,
      math::sin(rotation), math::cos(rotation), 0,
      0, 0, 1);

    Matrix m_scale(
      zoom, 0, 0,
      0, zoom, 0,
      0, 0, 1);

    Matrix m_translate(
      1, 0, offset.x,
      0, 1, offset.y,
      0, 0, 1);

    return m_origin * (m_scale * m_rotation) * m_translate;
  }

}