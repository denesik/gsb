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
  Magnum::Vector3 Unproject(Magnum::Vector2i pixel);

  Magnum::Matrix4 View();

private:
  Magnum::Quaternion mQuat;
  Magnum::Vector3 mPos;
  Magnum::Vector3 mDir;

  Magnum::Vector3 mUp = {0,1,0};
  Magnum::Vector3 mRight;
  Magnum::Vector3 mForward;

  Magnum::Vector2 mResolution;
  Magnum::Vector2 mFov;
};



#endif // Camera_h__
