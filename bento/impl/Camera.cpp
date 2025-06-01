#include "../Camera.hpp"

namespace ppx
{

  void Camera::Set(const Vec2 &offset, const Vec2 &target, const int16_t rotation, const f32 &zoom)
  {
    this->offset = offset;
    this->target = target;
    this->rotation = rotation;
    this->zoom = zoom;
  }

  Vec2 Camera::CameraToScreen(const Vec2 &position)
  {
    Matrix translate = Matrix::Translation(position.x, position.y);
    Matrix m = translate * GetMatrix();
    return Vec2(m[2], m[5]);
  }

  Vec2 Camera::ScreenToScreen(const Vec2 &position)
  {
    Matrix translate = Matrix::Translation(position.x, position.y);
    Matrix m = translate * Matrix::Inverse(GetMatrix());
    return Vec2(m[2], m[5]);
  }

  Matrix Camera::GetMatrix()
  {
    Matrix m_origin = Matrix::Translation(-target.x, -target.y);
    Matrix m_rotation = Matrix::Rotation(rotation);
    Matrix m_scale = Matrix::Scale(zoom, zoom);
    Matrix m_translate = Matrix::Translation(offset.x, offset.y);

    return m_origin * (m_scale * m_rotation) * m_translate;
  }

}