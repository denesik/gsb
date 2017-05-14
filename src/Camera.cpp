#include "Camera.h"
#include <Magnum/Math/Packing.h>
#include <Magnum/DefaultFramebuffer.h>
#include <StandartShader.h>
#include <DrawableArea.h>
#include <Magnum/Renderer.h>

using namespace Magnum;

void Camera::Reinit()
{
  mFov.x() = 60.0;
  switch (mType)
  {
  case Camera::Type::Perspective:
    mProject = Matrix4::perspectiveProjection(Deg(mFov.x()), mViewport.size().aspectRatio(), 0.01f, 1000.0f);
    break;
  case Camera::Type::Ortho:
    mProject = Matrix4::orthographicProjection(mViewport.size(), -500, 500);
    break;
  }
}

Camera::Camera(IMovable &movable, const Magnum::Range2Di &viewport, Camera::Type type)
  : mViewport(viewport)
  , mMovable(movable)
  , mType(type)
{
  Reinit();
}


Camera::~Camera()
{
}

void Camera::SetViewport(const Magnum::Range2Di &viewport)
{
  mViewport = static_cast<Magnum::Range2D>(viewport);
  Reinit();
}

Magnum::Vector3 Camera::Unproject(Magnum::Vector2 pixel, float depth) const
{
  auto Inverse = (Project() * View()).inverted();
  pixel.y() = -pixel.y() + mViewport.size().y();

  auto tmp = Vector4(pixel.x(), pixel.y(), depth, 1.f);
  tmp.x() = (tmp.x() - mViewport.min().x()) / mViewport.max().x();
  tmp.y() = (tmp.y() - mViewport.min().y()) / mViewport.max().y();
  tmp = tmp * 2.f - Vector4{ 1.f };

  auto obj = Inverse * tmp;
  obj /= obj.w();

  return obj.xyz();
}

Magnum::Vector3 Camera::Ray(Magnum::Vector2 pixel) const
{
  auto near = Unproject(pixel, 0.1f);
  auto far = Unproject(pixel, 1.f);
  return far - near;
}

Magnum::Matrix4 Camera::Project() const
{
  return mProject;
}

Magnum::Matrix4 Camera::View() const
{
  return mMovable.Model().inverted();
}

Magnum::Frustum Camera::Frustum() const
{
  return Math::Frustum<Float>::fromMatrix(Project() * View());
}

Magnum::Vector3 Camera::Pos() const
{
  return mMovable.Pos();
}



SunCamera::ShadowLayerData::ShadowLayerData(const Magnum::Vector2i& _size, const Magnum::Vector2& _orthoSize, Camera & parent)
  : shadowFramebuffer{ { {}, _size } }
  , size(_size)
  , orthographicSize(_orthoSize)
  , shadowMatrix(Matrix4::orthographicProjection(_orthoSize, orthographicNear, orthographicFar))
  , mParent(parent)
{
}

Magnum::Matrix4 SunCamera::ShadowLayerData::Project() const
{
  return shadowMatrix;
}

Magnum::Matrix4 SunCamera::ShadowLayerData::View() const
{
  return mParent.View();
}

Magnum::Frustum SunCamera::ShadowLayerData::Frustum() const
{
  return Math::Frustum<Float>::fromMatrix(Project() * View());
}

SunCamera::SunCamera(IMovable &movable, const Magnum::Range2Di &viewport, Camera::Type type, Texture2DArray & shadowTextureArray)
  : Camera(movable, viewport, type)
{
  for (std::int_fast32_t i = 0; i < StandartShader::ShadowMapLevels; ++i) 
  {
    _layers.emplace_back(viewport.size(), Magnum::Vector2(32.f * (i*4 + 1), 32.f * (i*4 + 1)), *this);
    auto &layer = GetLayer(i);
    layer.shadowFramebuffer.attachTextureLayer(Framebuffer::BufferAttachment::Depth, shadowTextureArray, 0, i)
      .mapForDraw(Framebuffer::DrawAttachment::None)
      .setViewport(viewport)
      .bind();
    CORRADE_INTERNAL_ASSERT(layer.shadowFramebuffer.checkStatus(FramebufferTarget::Draw) == Framebuffer::Status::Complete);
  }
}

SunCamera::ShadowLayerData & SunCamera::GetLayer(size_t number)
{
  return _layers[number];
}

void SunCamera::Draw(DrawableArea & area, ShadowShader & shader)
{
  for (size_t i = 0; i < StandartShader::ShadowMapLevels; i++)
  {
    _layers[i].shadowFramebuffer.clear(FramebufferClear::Depth).bind();
    Renderer::setColorMask(false, false, false, false);
    Renderer::setFaceCullingMode(Magnum::Renderer::PolygonFacing::Front);
    area.DrawShadowPass(_layers[i], shader);
  }
}

void SunCamera::SwitchLayerDebug()
{
  mCurrentLayerDebug++;
  if (mCurrentLayerDebug >= StandartShader::ShadowMapLevels)
    mCurrentLayerDebug = 0;
}

Magnum::Matrix4 SunCamera::Project() const
{
  return _layers[mCurrentLayerDebug].Project();
}

Magnum::Matrix4 SunCamera::View() const
{
  return Camera::View();
}
