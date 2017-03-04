#include "Camera.h"
#include <Magnum/Math/Packing.h>

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

Magnum::Vector3 Camera::Unproject(Magnum::Vector2i pixel)
{
  float mx = static_cast<float>((pixel.x() - mResolution.x() * 0.5) * (1.0 / mResolution.x()) * mFov.x() * 0.5);
  float my = static_cast<float>((pixel.y() - mResolution.y() * 0.5) * (1.0 / mResolution.x()) * mFov.x() * 0.5);
  Magnum::Vector3 dx = mRight * mx;
  Magnum::Vector3 dy = mUp * my;

   return (mForward + (dx + dy) * 2.0f).normalized();
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

  mForward = mQuat.transformVector({});
  mRight = Math::cross(mForward, mUp);

  return Matrix4::from(mQuat.toMatrix(), mPos);
}
