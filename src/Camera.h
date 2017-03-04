#pragma once
#ifndef Camera_h__
#define Camera_h__

#include <Magnum/Magnum.h>
#include <Magnum/Math/Quaternion.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Frustum.h>

class Camera
{
public:
  Camera();
  ~Camera();

  void SetResolution(const Magnum::Vector2i &size);

  void Rotate(const Magnum::Vector3 &dir);

  void Move(const Magnum::Vector3 &dist);

  Magnum::Vector3 Unproject(Magnum::Vector2i pixel);

  Magnum::Matrix4 Project();

  Magnum::Matrix4 View();
  Magnum::Matrix4 Projection() const;
  Magnum::Vector3 Position() const;

  Magnum::Frustum Frustum();

private:
  Magnum::Quaternion mQuat;
  Magnum::Vector3 mPos;
  Magnum::Vector3 mDir;

  Magnum::Vector3 mRight;
  Magnum::Vector3 mForward;

  Magnum::Vector2 mResolution;
  Magnum::Vector2 mFov;

  Magnum::Frustum mFrustum;
};



#endif // Camera_h__
