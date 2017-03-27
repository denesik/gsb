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

void Movable::Move(const Magnum::Vector3 &dist)
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

const Magnum::Matrix4 & Movable::Model() const
{
  return mModel;
}

const Magnum::Vector3 & Movable::Pos() const
{
  return mPos;
}

void Movable::Update()
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

