#include "Camera.h"
#include <Magnum/Math/Packing.h>
#include <Magnum/DefaultFramebuffer.h>

using namespace Magnum;

Camera::Camera()
{
  auto mat = Matrix4::lookAt
  (
    Vector3(0.0f, 0.0f, 0.0f), // eye
    Vector3(0.0f, 0.0f, 1.0f), // target
    Vector3(0.0f, 1.0f, 0.0f)  // up
  );
  mQuat = Quaternion::fromMatrix(mat.rotation());
  mProjection = Matrix4::perspectiveProjection(mFov, Vector2{ defaultFramebuffer.viewport().size() }.aspectRatio(), 0.01f, 1000.0f);
}


Camera::~Camera()
{
}

void Camera::Rotate(const Magnum::Vector3 &dir)
{
  mDir += dir;
}

void Camera::Move(const Magnum::Vector3 &dist)
{
  mPos += mQuat.transformVector(dist);
}

Magnum::Vector3 Camera::Unproject(Magnum::Vector2 pixel, float depth)
{
  auto Inverse = (Projection() * View()).inverted();
  auto viewport = defaultFramebuffer.viewport();
  pixel.y() = -pixel.y() + viewport.size().y();

  auto tmp = Vector4(pixel.x(), pixel.y(), depth, 1.f);
  tmp.x() = (tmp.x() - viewport.min().x()) / viewport.max().x();
  tmp.y() = (tmp.y() - viewport.min().y()) / viewport.max().y();
  tmp = tmp * 2.f - Vector4{1.f};

  auto obj = Inverse * tmp;
  obj /= obj.w();

  return obj.xyz();
}

Magnum::Vector3 Camera::Ray(Magnum::Vector2 pixel)
{
  auto near = Unproject(pixel, 0.1f);
  auto far = Unproject(pixel, 1.f);
  return far - near;
}

Magnum::Matrix4 Camera::View()
{
//   const auto &pitch = glm::angleAxis(mDir.x, glm::vec3(1, 0, 0));
//   const auto &yaw = glm::angleAxis(mDir.z, glm::vec3(0, 0, 1));
//   const auto &roll = glm::angleAxis(mDir.y, glm::vec3(0, 1, 0));
//   mDir = {};
// 
//   mQuat = pitch * mQuat * yaw;
//   mQuat = glm::normalize(mQuat);
// 
//   mView = glm::translate(glm::mat4_cast(mQuat), -mPos);

  auto pitch = Quaternion::rotation(Rad(mDir.y()), Vector3::xAxis());
  auto yaw = Quaternion::rotation(Rad(mDir.x()), Vector3::yAxis());
  auto roll = Quaternion::rotation(Rad(mDir.z()), Vector3::zAxis());
  mDir = {};

  mQuat = yaw* mQuat *pitch;
  mQuat = mQuat.normalized();
  auto matrix = Matrix4::from(mQuat.toMatrix(), mPos);

  mForward = -matrix.backward();
  mRight = matrix.right();
  mUp = matrix.up();

  return matrix;
}

Magnum::Matrix4 Camera::Projection() const
{
  return mProjection;
}

Magnum::Vector3 Camera::Position() const
{
  return mPos;
}
