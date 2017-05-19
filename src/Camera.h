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
#include <vector>
#include <Magnum\Framebuffer.h>

class ShadowShader;
class DrawableArea;

class ICamera
{
public:
  virtual Magnum::Matrix4 Project() const = 0;
  virtual Magnum::Matrix4 View() const = 0;

  virtual Magnum::Frustum Frustum() const = 0;
};

class Camera : public ICamera/* : public Movable*/
{
public:
  enum class Type
  {
    Perspective,
    Ortho
  };

  Camera(IMovable &movable, const Magnum::Range2Di &viewport, Type type = Type::Perspective);
  ~Camera();

  void SetViewport(const Magnum::Range2Di &viewport);

  Magnum::Vector3 Unproject(Magnum::Vector2 pixel, float depth) const;
  Magnum::Vector3 Ray(Magnum::Vector2 pixel) const;

  Magnum::Vector3 Pos() const;

  // Унаследовано через ICamera
  Magnum::Matrix4 Project() const override;
  Magnum::Matrix4 View() const override;
  Magnum::Frustum Frustum() const override;

protected:
  void Reinit();

  IMovable &mMovable;
  Type mType;

  Magnum::Range2D mViewport;
  Magnum::Vector2 mFov;

  Magnum::Frustum mFrustum;
  Magnum::Matrix4 mProject;
};

class SunCamera : public Camera
{
public:
  struct ShadowLayerData : ICamera {
    Magnum::Framebuffer shadowFramebuffer;
    Magnum::Vector2i size;
    Magnum::Vector2 orthographicSize;

    Magnum::Float orthographicNear = -500, orthographicFar = 500;

    Magnum::Matrix4 shadowMatrix;

    /*Magnum::Float cutPlane;
    Magnum::Matrix4 shadowCameraMatrix;*/

    explicit ShadowLayerData(const Magnum::Vector2i& size, const Magnum::Vector2& orthoSize, Camera &parent);

    // Унаследовано через ICamera
    Magnum::Matrix4 Project() const override;
    Magnum::Matrix4 View() const override;
    Magnum::Frustum Frustum() const override;

  private:
    Camera & mParent;
  };

  SunCamera(IMovable &movable, const Magnum::Range2Di &viewport, Camera::Type type, Magnum::Texture2DArray & shadowTextureArray);

  ShadowLayerData & GetLayer(size_t number);

  std::vector<ShadowLayerData> _layers;

  void Draw(DrawableArea & area, ShadowShader & shader);

  void SwitchLayerDebug();
  int mCurrentLayerDebug = 0;

  Magnum::Matrix4 Project() const override;
  Magnum::Matrix4 View() const override;
};

#endif // Camera_h__
