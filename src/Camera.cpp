#include "Camera.h"
#include <Magnum/Math/Functions.h>
#include <Magnum/Version.h>

template <typename T> int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

Magnum::Quaternion QuatFromMatrix(Magnum::Matrix4x4 m)
{
  float w = sqrt(std::max(0.f, 1 + m.data()[0, 0] + m.data()[1, 1] + m.data()[2, 2])) / 2;
  float x = sqrt(std::max(0.f, 1 + m.data()[0, 0] - m.data()[1, 1] - m.data()[2, 2])) / 2;
  float y = sqrt(std::max(0.f, 1 - m.data()[0, 0] + m.data()[1, 1] - m.data()[2, 2])) / 2;
  float z = sqrt(std::max(0.f, 1 - m.data()[0, 0] - m.data()[1, 1] + m.data()[2, 2])) / 2;
  x *= sgn(x * (m[2, 1] - m[1, 2]));
  y *= sgn(y * (m[0, 2] - m[2, 0]));
  z *= sgn(z * (m[1, 0] - m[0, 1]));

  return Magnum::Quaternion({ x, y, z }, w);
}

Magnum::Matrix4x4 MatrixFromQuat(Magnum::Quaternion q)
{
  float qw = q.scalar();
  float qx = q.vector().x;
  float qy = q.vector().y;
  float qz = q.vector().z;

  const float n = 1.0f / sqrt(qx*qx + qy*qy + qz*qz + qw*qw);
  qx *= n;
  qy *= n;
  qz *= n;
  qw *= n;

  return Magnum::Matrix4(
  { 1.0f - 2.0f*qy*qy - 2.0f*qz*qz, 2.0f*qx*qy - 2.0f*qz*qw, 2.0f*qx*qz + 2.0f*qy*qw, 0.0f },
  { 2.0f*qx*qy + 2.0f*qz*qw, 1.0f - 2.0f*qx*qx - 2.0f*qz*qz, 2.0f*qy*qz - 2.0f*qx*qw, 0.0f },
  { 2.0f*qx*qz - 2.0f*qy*qw, 2.0f*qy*qz + 2.0f*qx*qw, 1.0f - 2.0f*qx*qx - 2.0f*qy*qy, 0.0f },
  { 0.0f, 0.0f, 0.0f, 1.0f });
}

Magnum::Quaternion AngleAsis(float angle, Magnum::Vector3 a1)
{
  float s = sin(angle / 2);

  return{ {a1.x * s, a1.y * s,a1.z * s}, cos(angle / 2) };
}

Camera::Camera(void)
{
  RebuildProjection();

  mQuat = QuatFromMatrix(Magnum::Matrix4().lookAt(
    Magnum::Vector3(0.0f, 0.0f, 0.0f), // eye
    Magnum::Vector3(0.0f, 1.0f, 0.0f), // center
    Magnum::Vector3(0.0f, 0.0f, 1.0f)  // up
  ));
}

void Camera::LookAt(const Magnum::Vector3 &point)
{
  mQuat = QuatFromMatrix(Magnum::Matrix4().lookAt(
    mPos,
    point,
    Magnum::Vector3(0.0f, 0.0f, 1.0f)
  ));

  changed = true;
}

void Camera::RebuildProjection()
{
  switch (type)
  {
  case Camera::ORTHO:
  case Camera::SHADOW:
    mProjection = Magnum::Matrix4().orthographicProjection({ -100.f, 100.f }, -100.f, 100.f);
    break;
  case Camera::PERSPECTIVE:
    mProjection = Magnum::Matrix4().perspectiveProjection(mFov, mAspect, mNear, mFar);
    break;
  }
  changed = true;
}

void Camera::SetShadow()
{
  type = Camera::SHADOW;
  RebuildProjection();
}

void Camera::SetOrtho()
{
  type = Camera::ORTHO;
  RebuildProjection();
}

void Camera::SetPerspective()
{
  type = Camera::PERSPECTIVE;
  RebuildProjection();
}

Camera::~Camera(void)
{
}

const Magnum::Matrix4x4 &Camera::GetViewProject() const
{
  return mViewProjection;
}

const Magnum::Matrix4x4 & Camera::GetShadow() const
{
  return mShadow;
}

const Magnum::Matrix4x4 &Camera::GetView() const
{
  return mView;
}

const Magnum::Matrix4x4 &Camera::GetProject() const
{
  return mProjection;
}

const Magnum::Math::Matrix3<float> &Camera::GetDirection() const
{
  return mDirection;
}

void Camera::SetPos(const Magnum::Vector3 &pos)
{
  changed = true;
  mPos = pos;
}

const Magnum::Vector3 & Camera::GetPos() const
{
  return mPos;
}

void Camera::Resize(const Magnum::Vector2ui &size)
{
  mSize = size;
  changed = true;
  mAspect = static_cast<float>(size.x) / static_cast<float>(size.y);
  RebuildProjection();
}

void Camera::Rotate(const Magnum::Vector3 &degrees)
{
  changed = true;
  mDir += degrees;
}

void Camera::SetRot(const Magnum::Quaternion &quat)
{
  changed = true;
  mQuat = quat;
  mDir = {};
  Update();
}

void Camera::Move(const Magnum::Vector3 &dist)
{
  changed = true;
  mPos += Magnum::Vector3(dist.x, dist.z, -dist.y) * mQuat;
}

const Magnum::Matrix4 bias = Magnum::Matrix4(
{ 0.5f, 0.0f, 0.0f, 0.0f },
{ 0.0f, 0.5f, 0.0f, 0.0f },
{ 0.0f, 0.0f, 0.5f, 0.0f },
{ 0.5f, 0.5f, 0.5f, 1.0f });

void Camera::Update()
{
  if (!changed)
    return;

  auto pitch = AngleAsis(mDir.x, Magnum::Vector3(1, 0, 0));
  auto yaw = AngleAsis(mDir.z, Magnum::Vector3(0, 0, 1));
  //const auto roll = AngleAsis(mDir.y, Magnum::Vector3(0, 1, 0));
  mDir = {};

  mQuat = pitch * mQuat * yaw;
  mQuat = mQuat.normalized();

  
  mView = MatrixFromQuat(mQuat) * Magnum::Matrix4::translation(-mPos);;
  mViewProjection = mProjection * mView;

  mShadow = mViewProjection;

  //CalculateFrustum();
  changed = false;
}

//glm::ray Camera::GetRay(const Magnum::Vector2 &_pos)
//{
//  auto pos = _pos + Magnum::Vector2(0.5);
//  Magnum::Vector3 near = glm::unProject(Magnum::Vector3(pos.x, mSize.y - pos.y, 0.0f), mView, mProjection, Magnum::Vector4(0, 0, mSize.x, mSize.y));
//  Magnum::Vector3 far = glm::unProject(Magnum::Vector3(pos.x, mSize.y - pos.y, 1.0f), mView, mProjection, Magnum::Vector4(0, 0, mSize.x, mSize.y));
//  return glm::ray(near, far - near);
//}

void NormalizePlane(float frustum[6][4], int side) {
  float magnitude = (float)sqrt((frustum[side][0] * frustum[side][0]) +
    (frustum[side][1] * frustum[side][1]) +
    (frustum[side][2] * frustum[side][2]));
  frustum[side][0] /= magnitude;
  frustum[side][1] /= magnitude;
  frustum[side][2] /= magnitude;
  frustum[side][3] /= magnitude;
}

void Camera::CalculateFrustum() {
  const Magnum::Matrix4x4 &MV = mView;
  const Magnum::Matrix4x4 &P = mProjection;

  m_clipMatrix[0] = (MV[0][0] * P[0][0]) + (MV[0][1] * P[1][0]) + (MV[0][2] * P[2][0]) + (MV[0][3] * P[3][0]);
  m_clipMatrix[1] = (MV[0][0] * P[0][1]) + (MV[0][1] * P[1][1]) + (MV[0][2] * P[2][1]) + (MV[0][3] * P[3][1]);
  m_clipMatrix[2] = (MV[0][0] * P[0][2]) + (MV[0][1] * P[1][2]) + (MV[0][2] * P[2][2]) + (MV[0][3] * P[3][2]);
  m_clipMatrix[3] = (MV[0][0] * P[0][3]) + (MV[0][1] * P[1][3]) + (MV[0][2] * P[2][3]) + (MV[0][3] * P[3][3]);

  m_clipMatrix[4] = (MV[1][0] * P[0][0]) + (MV[1][1] * P[1][0]) + (MV[1][2] * P[2][0]) + (MV[1][3] * P[3][0]);
  m_clipMatrix[5] = (MV[1][0] * P[0][1]) + (MV[1][1] * P[1][1]) + (MV[1][2] * P[2][1]) + (MV[1][3] * P[3][1]);
  m_clipMatrix[6] = (MV[1][0] * P[0][2]) + (MV[1][1] * P[1][2]) + (MV[1][2] * P[2][2]) + (MV[1][3] * P[3][2]);
  m_clipMatrix[7] = (MV[1][0] * P[0][3]) + (MV[1][1] * P[1][3]) + (MV[1][2] * P[2][3]) + (MV[1][3] * P[3][3]);

  m_clipMatrix[8] = (MV[2][0] * P[0][0]) + (MV[2][1] * P[1][0]) + (MV[2][2] * P[2][0]) + (MV[2][3] * P[3][0]);
  m_clipMatrix[9] = (MV[2][0] * P[0][1]) + (MV[2][1] * P[1][1]) + (MV[2][2] * P[2][1]) + (MV[2][3] * P[3][1]);
  m_clipMatrix[10] = (MV[2][0] * P[0][2]) + (MV[2][1] * P[1][2]) + (MV[2][2] * P[2][2]) + (MV[2][3] * P[3][2]);
  m_clipMatrix[11] = (MV[2][0] * P[0][3]) + (MV[2][1] * P[1][3]) + (MV[2][2] * P[2][3]) + (MV[2][3] * P[3][3]);

  m_clipMatrix[12] = (MV[3][0] * P[0][0]) + (MV[3][1] * P[1][0]) + (MV[3][2] * P[2][0]) + (MV[3][3] * P[3][0]);
  m_clipMatrix[13] = (MV[3][0] * P[0][1]) + (MV[3][1] * P[1][1]) + (MV[3][2] * P[2][1]) + (MV[3][3] * P[3][1]);
  m_clipMatrix[14] = (MV[3][0] * P[0][2]) + (MV[3][1] * P[1][2]) + (MV[3][2] * P[2][2]) + (MV[3][3] * P[3][2]);
  m_clipMatrix[15] = (MV[3][0] * P[0][3]) + (MV[3][1] * P[1][3]) + (MV[3][2] * P[2][3]) + (MV[3][3] * P[3][3]);

  m_frustum[0][0] = m_clipMatrix[3] - m_clipMatrix[0];
  m_frustum[0][1] = m_clipMatrix[7] - m_clipMatrix[4];
  m_frustum[0][2] = m_clipMatrix[11] - m_clipMatrix[8];
  m_frustum[0][3] = m_clipMatrix[15] - m_clipMatrix[12];
  NormalizePlane(m_frustum, 0);

  m_frustum[1][0] = m_clipMatrix[3] + m_clipMatrix[0];
  m_frustum[1][1] = m_clipMatrix[7] + m_clipMatrix[4];
  m_frustum[1][2] = m_clipMatrix[11] + m_clipMatrix[8];
  m_frustum[1][3] = m_clipMatrix[15] + m_clipMatrix[12];
  NormalizePlane(m_frustum, 1);

  m_frustum[2][0] = m_clipMatrix[3] + m_clipMatrix[1];
  m_frustum[2][1] = m_clipMatrix[7] + m_clipMatrix[5];
  m_frustum[2][2] = m_clipMatrix[11] + m_clipMatrix[9];
  m_frustum[2][3] = m_clipMatrix[15] + m_clipMatrix[13];
  NormalizePlane(m_frustum, 2);

  m_frustum[3][0] = m_clipMatrix[3] - m_clipMatrix[1];
  m_frustum[3][1] = m_clipMatrix[7] - m_clipMatrix[5];
  m_frustum[3][2] = m_clipMatrix[11] - m_clipMatrix[9];
  m_frustum[3][3] = m_clipMatrix[15] - m_clipMatrix[13];
  NormalizePlane(m_frustum, 3);

  m_frustum[4][0] = m_clipMatrix[3] - m_clipMatrix[2];
  m_frustum[4][1] = m_clipMatrix[7] - m_clipMatrix[6];
  m_frustum[4][2] = m_clipMatrix[11] - m_clipMatrix[10];
  m_frustum[4][3] = m_clipMatrix[15] - m_clipMatrix[14];
  NormalizePlane(m_frustum, 4);

  m_frustum[5][0] = m_clipMatrix[3] + m_clipMatrix[2];
  m_frustum[5][1] = m_clipMatrix[7] + m_clipMatrix[6];
  m_frustum[5][2] = m_clipMatrix[11] + m_clipMatrix[10];
  m_frustum[5][3] = m_clipMatrix[15] + m_clipMatrix[14];
  NormalizePlane(m_frustum, 5);
}

bool Camera::BoxWithinFrustum(const Magnum::Vector4 &min, const Magnum::Vector4 &max) const {
  for (int i = 0; i < 6; i++) {
    if ((m_frustum[i][0] * min.x + m_frustum[i][1] * min.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * min.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * min.x + m_frustum[i][1] * max.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * max.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * min.x + m_frustum[i][1] * min.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * min.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * min.x + m_frustum[i][1] * max.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * max.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F))
    {
      return false;
    }
  }
  return true;
}

bool Camera::BoxWithinFrustum(const Magnum::Vector3 &min, const Magnum::Vector3 &max) const {
  for (int i = 0; i < 6; i++) {
    if ((m_frustum[i][0] * min.x + m_frustum[i][1] * min.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * min.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * min.x + m_frustum[i][1] * max.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * max.y + m_frustum[i][2] * min.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * min.x + m_frustum[i][1] * min.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * min.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * min.x + m_frustum[i][1] * max.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F) &&
      (m_frustum[i][0] * max.x + m_frustum[i][1] * max.y + m_frustum[i][2] * max.z + m_frustum[i][3] <= 0.0F))
    {
      return false;
    }
  }
  return true;
}

//glm::ray Camera::Unproject(const Magnum::Vector2 pos) const
//{
//  glm::uvec4 v = { 0.f, 0.f, mSize.x, mSize.y };
//  Magnum::Vector3 near = glm::unProject(Magnum::Vector3(pos.x, mSize.x - pos.y, 0.f), GetView(), GetProject(), v);
//  Magnum::Vector3 far = glm::unProject(Magnum::Vector3(pos.x, mSize.y - pos.y, 1.f), GetView(), GetProject(), v);
//  glm::ray ray(near, far - near);
//  return ray;
//}
//
//Magnum::Vector2 Camera::Project(const Magnum::Vector3 &pos) const
//{
//  glm::uvec4 vi = { 0.f, 0.f, mSize.x, mSize.y };
//  Magnum::Vector3 v = glm::project(pos, GetView(), GetProject(), vi);
//  //v /= v.z;
//  return Magnum::Vector2(v.x, mSize.y - v.y);
//}