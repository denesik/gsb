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

void Movable::Rotate(const Magnum::Vector3 &dir)
{
  mDir += dir;
}

void Movable::Move(const Magnum::Vector3 &dist)
{
  mDeltaPos += mQuat.transformVector(dist);
}

void Movable::LookAt(const Magnum::Vector3 &target)
{
  mQuat = Quaternion::fromMatrix(Matrix4::lookAt(mPos, target, mModel.up()).rotation());
}

void Movable::SetPos(const Magnum::Vector3 &pos)
{
  mPos = pos;
  mModel = Matrix4::from(mQuat.toMatrix(), mPos); // ???
  mDeltaPos = {};
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
  auto pitch = Quaternion::rotation(Rad(mDir.y()), Vector3::xAxis());
  auto yaw = Quaternion::rotation(Rad(mDir.x()), Vector3::yAxis());
  auto roll = Quaternion::rotation(Rad(mDir.z()), Vector3::zAxis());
  mDir = {};

  mQuat = yaw* mQuat *pitch;
  mQuat = mQuat.normalized();
  mPos += mDeltaPos;
  mDeltaPos = {};

  mModel = Matrix4::from(mQuat.toMatrix(), mPos);
}

