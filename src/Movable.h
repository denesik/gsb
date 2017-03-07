#pragma once
#ifndef Transformation_h__
#define Transformation_h__

#include <Magnum/Magnum.h>
#include <Magnum/Math/Quaternion.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Matrix4.h>


class Movable
{
public:
  Movable();
  ~Movable();

  void Rotate(const Magnum::Vector3 &dir);
  void Move(const Magnum::Vector3 &dist);
  void LookAt(const Magnum::Vector3 &target);

  void SetPos(const Magnum::Vector3 &pos);

  const Magnum::Matrix4 &Model() const;
  const Magnum::Vector3 &Pos() const;

  void Update();

protected:
  Magnum::Quaternion mQuat;
  Magnum::Vector3 mPos;
  Magnum::Vector3 mDeltaPos;
  Magnum::Vector3 mDir;
  Magnum::Matrix4 mModel;

};



#endif // Transformation_h__
