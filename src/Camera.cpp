#include "Camera.h"
#include <Magnum/Math/Packing.h>
#include <Magnum/DefaultFramebuffer.h>

using namespace Magnum;

Camera::Camera(Movable &movable, const Magnum::Range2Di &viewport, Camera::Type type) 
  : mViewport(viewport)
  , mMovable(movable)
{
  mFov.x() = 60.0;
  switch (type)
  {
  case Camera::Type::Perspective:
    mProject = Matrix4::perspectiveProjection(Deg(mFov.x()), mViewport.size().aspectRatio(), 0.01f, 1000.0f);
    break;
  case Camera::Type::Ortho:
    mProject = Matrix4::orthographicProjection(Vector2({350, 350}), -500, 500);
    break;
  }
}


Camera::~Camera()
{
}

void Camera::SetViewport(const Magnum::Range2Di &viewport)
{
  mViewport = static_cast<Magnum::Range2D>(viewport);
}

Magnum::Vector3 Camera::Unproject(Magnum::Vector2 pixel, float depth) const
{
  auto Inverse = (Project() * View()).inverted();
  pixel.y() = -pixel.y() + mViewport.size().y();

  auto tmp = Vector4(pixel.x(), pixel.y(), depth, 1.f);
  tmp.x() = (tmp.x() - mViewport.min().x()) / mViewport.max().x();
  tmp.y() = (tmp.y() - mViewport.min().y()) / mViewport.max().y();
  tmp = tmp * 2.f - Vector4{1.f};

  auto obj = Inverse * tmp;
  obj /= obj.w();

  return obj.xyz();
}

Magnum::Vector3 Camera::Ray(Magnum::Vector2 pixel) const
{
  auto near = Unproject(pixel, 0.1f);
  auto far = Unproject(pixel, 1.f);
  return far - near;
}

Magnum::Matrix4 Camera::Project() const
{
  return mProject;
}

Magnum::Matrix4 Camera::View() const
{
  return mMovable.Model().inverted();
}

Magnum::Frustum Camera::Frustum() const
{
  return Math::Frustum<Float>::fromMatrix(Project() * View());
}
