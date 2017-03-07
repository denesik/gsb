#include "DrawableArea.h"
#include "World.h"
#include "tools\CoordSystem.h"

#include <Magnum/Math/Frustum.h>
#include "StandartShader.h"
#include <Magnum/Math/Geometry/Intersection.h>
#include <Magnum/Math/Frustum.h>

using namespace Magnum;

DrawableArea::DrawableArea(World &world, const SPos &pos, unsigned int radius)
  : mWorld(world), mPos(pos)
{
  mSectorCompiler = std::make_shared<SectorCompiler>(world.GetBlocksDataBase());
  UpdateRadius(radius);
  UpdatePos(mPos);
}


DrawableArea::~DrawableArea()
{
}

void DrawableArea::SetRadius(unsigned int radius)
{
  UpdateRadius(radius);
  UpdatePos(mPos);
}

void DrawableArea::SetPos(const SPos &pos)
{
  if (mPos != pos)
  {
    mPos = pos;
    UpdatePos(mPos);
  }
}

void DrawableArea::Draw(Camera &camera, Magnum::AbstractShaderProgram& shader)
{
  // Обновляем список видимых секторов N раз в сек.

  const double N = 10.0;
  bool loading = false;
  if (mTimer.Elapsed() > 1.0 / N)
  {
    mTimer.Start();
    loading = true;
  }
  loading = true;

  const auto &frustum = camera.Frustum(); 

  for (auto &site : mSectors)
  {
    auto sector = std::get<2>(site).lock();
    auto &sector_compiler = std::get<3>(site);
    auto &sector_render_data = std::get<4>(site);

    if (loading && !sector)
    {
      std::get<2>(site) = mWorld.GetSector(std::get<1>(site));
      sector = std::get<2>(site).lock();
    }

    if (sector)
    {
      // Если для этого сектора включен компилятор
      // Если он завершил работу обновляем меш
      if (sector_compiler)
      {
        if (sector_compiler->IsDone())
        {
          if (!sector_render_data) sector_render_data = std::make_unique<SectorRenderData>();
          sector_render_data->SetPos(std::get<1>(site));
          sector_render_data->vertex_buffer.setData(sector_compiler->GetVertexData(), BufferUsage::StaticDraw);
          sector_render_data->index_buffer.setData(sector_compiler->GetIndexData(), BufferUsage::StaticDraw);
          sector_render_data->mesh.setCount(sector_compiler->GetIndexData().size());
          sector_compiler.reset();
        }
      }
      // Если компилятора нет, проверяем нужно ли скомпилировать сектор.
      else
      {
        if ((sector->NeedCompile() || !sector_render_data) && mSectorCompiler.use_count() == 1)
        {
          sector->NeedCompile(false);
          sector_compiler = mSectorCompiler;
          sector->SetCompilerData(*sector_compiler);
          sector_compiler->Run();
        }
      }

      if (sector_render_data) sector_render_data->Draw(frustum, camera.Project() * camera.View(), shader);
    }
    else if (sector_render_data)
    {
      sector_render_data.reset();
    }
  }

}

void DrawableArea::UpdateRadius(unsigned int radius)
{
  mSectors.clear();

  int begin = -static_cast<int>(radius);
  int end = static_cast<int>(radius);
  SPos pos(begin);
  for (pos.z() = begin; pos.z() <= end; ++pos.z())
    for (pos.y() = 0; pos.y() < SECTOR_COUNT_HEIGHT; ++pos.y())
      for (pos.x() = begin; pos.x() <= end; ++pos.x())
      {
        mSectors.emplace_back(pos, SPos{}, std::weak_ptr<Sector>{}, nullptr, nullptr);
      }
}

void DrawableArea::UpdatePos(const SPos &pos)
{
  for (auto &site : mSectors)
  {
    std::get<1>(site) = std::get<0>(site) + pos;
    if (std::get<4>(site)) std::get<4>(site)->SetPos(std::get<1>(site));
    std::get<2>(site).reset();
  }
}

SectorRenderData::SectorRenderData()
{
  mesh.setPrimitive(MeshPrimitive::Triangles);
  mesh.addVertexBuffer(vertex_buffer, 0, StandartShader::Position{}, StandartShader::TextureCoordinates{});
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
  aabb.max() = wpos + WPos(static_cast<WPosType>(SECTOR_SIZE));
}

void SectorRenderData::Draw(const Magnum::Frustum &frustum, const Magnum::Matrix4 &matrix, Magnum::AbstractShaderProgram& shader)
{
  if (Math::Geometry::Intersection::boxFrustum(aabb, frustum))
  {
    static_cast<StandartShader &>(shader).setProjection(matrix * model);
    mesh.draw(shader);
  }
}
