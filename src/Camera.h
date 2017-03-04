#pragma once
#ifndef Camera_h__
#define Camera_h__

#include <Magnum/Magnum.h>
#include <Magnum/Math/Quaternion.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Matrix4.h>


class Camera
{
public:
  Camera();
  ~Camera();

  void Rotate(const Magnum::Vector3 &dir);

  void Move(const Magnum::Vector3 &dist);
  Magnum::Vector3 Unproject(Magnum::Vector2 pixel, float depth);
  Magnum::Vector3 Ray(Magnum::Vector2 pixel);

  Magnum::Matrix4 View();
  Magnum::Matrix4 Projection() const;
  Magnum::Vector3 Position() const;

public:
  Magnum::Quaternion mQuat;
  Magnum::Vector3 mPos;
  Magnum::Vector3 mDir;

  Magnum::Vector3 mRight;
  Magnum::Vector3 mForward;
  Magnum::Vector3 mUp;

  Magnum::Rad mFov = Magnum::Deg(60);

  Magnum::Matrix4 mProjection;
};



#endif // Camera_h__
