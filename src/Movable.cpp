#include "Movable.h"

using namespace Magnum;


Movable::Movable()
{
  mQuat = Quaternion::fromMatrix(Matrix4::lookAt
  (
    Vector3(0.0f, 0.0f, 0.0f), // eye
    Vector3(0.0f, 0.0f, 1.0f), // target
    Vector3(0.0f, 1.0f, 0.0f)  // up
  ).rotation());
}


Movable::~Movable()
{
}

void Movable::Rotate(const Magnum::Vector3 &euler)
{
  mNextRotation += euler;
}

void Movable::MoveRelative(const Magnum::Vector3 &dist)
{
  mNextMove += mQuat.transformVector(dist);
}

void Movable::LookAt(const Magnum::Vector3 &target)
{
  //mQuat = Quaternion::fromMatrix(Matrix4::lookAt(mPos, target, mModel.up()).rotation());
	mEye = target;
    mQuat = Quaternion::fromMatrix(Matrix4::lookAt(mPos, target, Vector3(0.0f, 1.0f, 0.0f)).rotation());
}

void Movable::SetPos(const Magnum::Vector3 &pos)
{
  mPos = pos;
  mModel = Matrix4::from(mQuat.toMatrix(), mPos); // ???
  mNextMove = {};
}

const Magnum::Matrix4 Movable::Model() const
{
  return mModel;
}

const Magnum::Vector3 Movable::Pos() const
{
  return mPos;
}

void Movable::Update(const Magnum::Timeline &gt)
{
  auto pitch = Quaternion::rotation(Rad(mNextRotation.y()), Vector3::xAxis());
  auto yaw = Quaternion::rotation(Rad(mNextRotation.x()), Vector3::yAxis());
  auto roll = Quaternion::rotation(Rad(mNextRotation.z()), Vector3::zAxis());
  mNextRotation = {};

  mQuat = yaw* mQuat *pitch;
  mQuat = mQuat.normalized();
  mPos += mNextMove;
  mNextMove = {};

  mModel = Matrix4::from(mQuat.toMatrix(), mPos);
}

const Magnum::Vector3 Movable::Direction() const
{
	return (mEye - mPos).normalized();
}

void Acceleratable::Accelerate(const Magnum::Vector3 & velocity)
{
  mVelocity += velocity;
}

void Acceleratable::SetAcceleration(const Magnum::Vector3 & velocity)
{
  mVelocity = velocity;
}

void Acceleratable::Update(const Magnum::Timeline & gt)
{
  mVelocity += mGravity * gt.previousFrameDuration();
  mPos += mVelocity * gt.previousFrameDuration();
  Movable::Update(gt);
}

Magnum::Vector3 &Acceleratable::Velocity()
{
  return mVelocity;
}

void Acceleratable::Stop()
{
  mVelocity = {};
}

MovableOffseted::MovableOffseted(Movable & movable, const Magnum::Vector3 & off) 
  : mReference(movable)
  , mOffset(off)
{
}

const Magnum::Matrix4 MovableOffseted::Model() const
{
  return mReference.Model() * Matrix4::translation(mOffset);
}

const Magnum::Vector3 MovableOffseted::Pos() const
{
  return mReference.Pos() + mOffset;
}

void MovableOffseted::Update(const Magnum::Timeline &gt)
{
  mReference.Update(gt);
}

const Magnum::Vector3 MovableOffseted::Direction() const
{
  return mReference.Direction();
}