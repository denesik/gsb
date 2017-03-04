#pragma once

#include <memory>
#include <Magnum/Math/Vector.h>
#include <Magnum/Math/Matrix.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Math/Quaternion.h>
#include <Magnum/Math/Math.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>

class Camera;
typedef std::shared_ptr<Camera> PCamera;
typedef std::shared_ptr<const Camera> CPCamera;

class Camera
{
public:
  Camera();
  ~Camera();

  enum Type {
    ORTHO,
    SHADOW,
    PERSPECTIVE
  };

  const Magnum::Matrix4x4 & GetViewProject() const;

  const Magnum::Matrix4x4 & GetShadow() const;

  // ѕолучить матрицу вида.
  const Magnum::Matrix4x4 &GetView() const;

  // ѕолучить матрицу проекции.
  const Magnum::Matrix4x4 &GetProject() const;

  const Magnum::Math::Matrix3<float> &GetDirection() const;

  void SetPos(const Magnum::Vector3 &pos);

  void LookAt(const Magnum::Vector3 & point);

  const Magnum::Vector3 &GetPos() const;

  void Resize(const Magnum::Vector2ui &size);

  // ѕовернуть камеру относительно текущей ориентации на заданный угол по трем ос€м.
  void Rotate(const Magnum::Vector3 &angle);

  void SetRot(const Magnum::Quaternion &quat);

  // ѕереместитьс€, относительно текущего положени€ и ориентации.
  void Move(const Magnum::Vector3 &dist);

  void Update();

  //glm::ray GetRay(const Magnum::Vector2 &pos);

  void CalculateFrustum();

  bool BoxWithinFrustum(const Magnum::Vector4 & min, const Magnum::Vector4 & max) const;

  bool BoxWithinFrustum(const Magnum::Vector3 & min, const Magnum::Vector3 & max) const;

  //Magnum::Math:: Unproject(const Magnum::Vector2 pos) const;

  //Magnum::Vector2 Project(const Magnum::Vector3 & pos) const;

  void SetPerspective();
  void SetOrtho();
  void RebuildProjection();
  void SetShadow();
  Type type = PERSPECTIVE;

private:
  Magnum::Math::Quaternion<float> mQuat;

  Magnum::Matrix4x4 mView;
  Magnum::Matrix4x4 mProjection;
  Magnum::Matrix4x4 mViewProjection;
  Magnum::Matrix4x4 mShadow;
  Magnum::Vector3 mDirection;

  Magnum::Vector3 mDir;
  Magnum::Vector3 mPos;

  float m_clipMatrix[16];
  float m_frustum[6][4];

  Magnum::Rad mFov = Magnum::Rad(45.0);
  float mAspect = 1.0f;
  float mNear = 0.01f;
  float mFar = 2000.f;
  Magnum::Vector2ui mSize = { 600, 600 };

  bool changed = true;
};