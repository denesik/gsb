#pragma once
#ifndef Transformation_h__
#define Transformation_h__

#include <Magnum/Magnum.h>
#include <Magnum/Math/Quaternion.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Timeline.h>

class IMovable
{
public:
  virtual const Magnum::Matrix4 Model() const = 0;
  virtual const Magnum::Vector3 Pos() const = 0; 
  virtual const Magnum::Vector3 Direction() const = 0;

  virtual void Update(const Magnum::Timeline &gt) = 0;
};

class Movable : public IMovable
{
public:
  Movable();
  ~Movable();

  void Rotate(const Magnum::Vector3 &dir);
  void MoveRelative(const Magnum::Vector3 &dist);
  void LookAt(const Magnum::Vector3 &target);

  void SetPos(const Magnum::Vector3 &pos);

  virtual const Magnum::Matrix4 Model() const;
  virtual const Magnum::Vector3 Pos() const;
  virtual const Magnum::Vector3 Direction() const;

  virtual void Update(const Magnum::Timeline &gt);

protected:
  Magnum::Quaternion mQuat;
  Magnum::Vector3 mPos;
  Magnum::Vector3 mNextMove;
  Magnum::Vector3 mNextRotation;
  Magnum::Matrix4 mModel;
  Magnum::Vector3 mEye;
};

class MovableOffseted : public IMovable
{
public:

  MovableOffseted(Movable &movable, const Magnum::Vector3 &off);

  virtual const Magnum::Matrix4 Model() const;
  virtual const Magnum::Vector3 Pos() const;
  virtual const Magnum::Vector3 Direction() const;

  virtual void Update(const Magnum::Timeline &gt);

protected:
  Magnum::Vector3 mOffset;
  Movable &mReference;
};

class Acceleratable : public Movable
{
public:
  void Accelerate(const Magnum::Vector3 &velocity);
  void SetAcceleration(const Magnum::Vector3 & velocity);
  void Update(const Magnum::Timeline &gt) override;

  Magnum::Vector3 &Velocity();
  void Stop();

protected:
  Magnum::Vector3 mVelocity;
  Magnum::Vector3 mGravity = {0, 0/*-9.8f*/, 0};
};

#endif // Transformation_h__
