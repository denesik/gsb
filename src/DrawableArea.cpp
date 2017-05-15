#include "DrawableArea.h"
#include "World.h"
#include "tools\CoordSystem.h"

#include <Magnum/Math/Frustum.h>
#include "StandartShader.h"
#include <Magnum/Math/Geometry/Intersection.h>
#include <Magnum/Math/Frustum.h>

using namespace Magnum;

DrawableArea::DrawableArea(World &world, const SPos &pos, unsigned int radius)
  : mWorld(world), mPos(pos), 
  mCompiler(1, 1, world.GetBlocksDataBase()),
  mData({1, 1})
{
  mData.onAdding = [this](const SPos &pos)
  {
    OnSectorAdd(pos);
  };

  mData.onDeletting = [this](const SPos &pos)
  {
    OnSectorRemove(pos);
  };
}


DrawableArea::~DrawableArea()
{
}

void DrawableArea::SetRadius(unsigned int radius)
{
}

void DrawableArea::SetPos(const SPos &pos)
{
  mData.UpdatePos(pos);
}

// TODO: Не компилировать сектор, если он компилируется в данный момент.
// void DrawableArea::DrawShadowPass(const Camera & sun, ShadowShader & shader)
// {
//   // Обновляем список видимых секторов N раз в сек.
// 
//   const double N = 10.0;
//   bool loading = false;
//   if (mTimer.Elapsed() > 1.0 / N)
//   {
//     mTimer.Start();
//     loading = true;
//   }
//   loading = true;
// 
//   const auto &frustum = sun.Frustum();
//   const auto &sun_matrix = sun.Project() * sun.View();
// 
//   for (auto &data : mData)
//   {
//     if (data.sector.expired())
//     {
//       if (loading)
//       {
//         data.sector = mWorld.GetSector(data.world_pos);
//       }
//       data.drawable->valide = false;
//       data.drawable->compile = false;
//     }
// 
//     if (data.drawable->valide)
//     {
//       data.drawable->DrawShadowPass(frustum, sun_matrix, shader);
//     }
// 
//     if (!data.sector.expired())
//     {
//       auto sector = data.sector.lock();
//       if (sector->NeedCompile() /*|| !data.drawable->compile*/)
//       {
//         data.drawable->compile = true;
//         sector->NeedCompile(false);
//         mCompilerWorker.Add({ data.sector, data.drawable });
//       }
// 
//       //sector->Draw(matrix, shader);
//     }
//   }
// 
//   mCompilerWorker.Update();
// }

void DrawableArea::Draw(const Camera & camera, const Camera & sun, const Vector3 &lightdir, StandartShader & shader)
{
  // Обновляем список видимых секторов N раз в сек.

//   const double N = 10.0;
//   bool loading = false;
//   if (mTimer.Elapsed() > 1.0 / N)
//   {
//     mTimer.Start();
//     loading = true;
//   }
//   loading = true;

  const auto &frustum = camera.Frustum();
  const auto &matrix = camera.Project() * camera.View();
  const auto &sun_matrix = sun.Project() * sun.View();

  for (auto &data : mData)
  {
    //Каждый кадр бегаем по всем элементам, если сектор нужно скомпилить, отправляем его на компиляцию.
    if (data.sector)
    {
      if (data.sector->NeedCompile())
      {
        data.sector->NeedCompile(false);
        mCompiler.Push(data.sector->GetPos());
      }

      data.drawable.Draw(frustum, matrix, sun_matrix, lightdir, shader);
    }
  }

  mCompiler.Update();
}

void DrawableArea::OnSectorLoadBegin(Worker &worker, SPos &pos)
{
  auto &sector = mData.Get(pos); 
  if (sector && sector->sector)
    sector->sector->SetCompilerData(worker.compiler);
}

void DrawableArea::OnSectorLoadEnd(Worker &worker, SPos &pos)
{

}

void DrawableArea::OnSectorAdd(const SPos &pos)
{
//   auto &sector = mData.Get(pos);
//   if (sector)
//     sector->sector->SetCompilerData(worker.compiler);
}

void DrawableArea::OnSectorRemove(const SPos &pos)
{

}

//=============== SectorRenderData ===============
SectorRenderData::SectorRenderData()
{
  mesh.setPrimitive(MeshPrimitive::Triangles);
  mesh.addVertexBuffer(vertex_buffer, 0, StandartShader::Position{}, StandartShader::TextureCoordinates{}, StandartShader::Normal{});
  mesh.setIndexBuffer(index_buffer, 0, Mesh::IndexType::UnsignedInt);
}

void SectorRenderData::SetPos(const SPos &pos)
{
  WPos wpos = cs::StoW(pos);
  model = {};
  model = model * Math::Matrix4<Float>::translation(Vector3::xAxis(wpos.x()));
  model = model * Math::Matrix4<Float>::translation(Vector3::yAxis(wpos.y()));
  model = model * Math::Matrix4<Float>::translation(Vector3::zAxis(wpos.z()));

  aabb.min() = wpos;
  aabb.max() = wpos + WPos(static_cast<WPosType>(gSectorSize.x()), static_cast<WPosType>(gSectorSize.y()), static_cast<WPosType>(gSectorSize.z()));
}

void SectorRenderData::Draw(const Magnum::Frustum &frustum, const Magnum::Matrix4 &matrix, const Magnum::Matrix4 &sun_matrix, const Magnum::Vector3 &lightdir, StandartShader& shader)
{
  constexpr const Matrix4 bias
  { { 0.5f, 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.5f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.5f, 0.0f },
  { 0.5f, 0.5f, 0.5f, 1.0f } };

  if (Math::Geometry::Intersection::boxFrustum(aabb, frustum))
  {
    shader.setProjection(matrix * model);
    shader.setLightVector(lightdir);
    shader.setShadowMatrix(bias * sun_matrix * model);
    mesh.draw(shader);
  }
}

void SectorRenderData::DrawShadowPass(const Magnum::Frustum &frustum, const Magnum::Matrix4 &matrix, ShadowShader& shader)
{
  if (Math::Geometry::Intersection::boxFrustum(aabb, frustum))
  {
    shader.setProjection(matrix * model);
    mesh.draw(shader);
  }
}


// auto drawable = mDrawable.lock();
// drawable->SetPos(mPos);
// drawable->vertex_buffer.setData(compiler.GetVertexData(), BufferUsage::StaticDraw);
// drawable->index_buffer.setData(compiler.GetIndexData(), BufferUsage::StaticDraw);
// drawable->mesh.setCount(compiler.GetIndexData().size());
// drawable->valide = true;
