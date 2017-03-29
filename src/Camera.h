#pragma once
#ifndef Camera_h__
#define Camera_h__

#include <Magnum/Magnum.h>
#include <Magnum/Math/Quaternion.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Frustum.h>
#include <Magnum/Math/Vector2.h>
#include "Movable.h"
#include <Magnum/Math/Range.h>

class Camera/* : public Movable*/
{
public:
  Camera(Movable &movable);
  ~Camera();

  void SetViewport(const Magnum::Range2Di &viewport);

  Magnum::Vector3 Unproject(Magnum::Vector2 pixel, float depth) const;
  Magnum::Vector3 Ray(Magnum::Vector2 pixel) const;

  Magnum::Matrix4 Project() const;
  Magnum::Matrix4 View() const;

  Magnum::Frustum Frustum() const;

private:
  Movable &mMovable;

  Magnum::Range2D mViewport;
  Magnum::Vector2 mFov;

  Magnum::Frustum mFrustum;
};



#endif // Camera_h__
