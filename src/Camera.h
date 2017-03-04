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

  Magnum::Matrix4 View();

private:
  Magnum::Quaternion mQuat;
  Magnum::Vector3 mPos;
  Magnum::Vector3 mDir;

};



#endif // Camera_h__
