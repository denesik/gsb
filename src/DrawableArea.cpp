#include "DrawableArea.h"
#include "World.h"
#include "tools\CoordSystem.h"

#include <Magnum/Math/Frustum.h>
#include "StandartShader.h"
#include <Magnum/Math/Geometry/Intersection.h>
#include <Magnum/Math/Frustum.h>

using namespace Magnum;

DrawableArea::DrawableArea(World &world, const SPos &pos, unsigned int radius)
  : mWorld(world), mPos(pos), mCompilerWorker(world.GetBlocksDataBase())
{
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

//TODO: Не компилировать сектор, если он компилируется в данный момент.
void DrawableArea::Draw(const Camera &camera, const Camera &sun, Magnum::AbstractShaderProgram& shader)
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
  const auto &matrix = camera.Project() * camera.View();
  const auto &sun_matrix = sun.Project() * sun.View();

  for (auto &data : mData)
  {
    if (data.sector.expired())
    {
      if (loading)
      {
        data.sector = mWorld.GetSector(data.world_pos);
      }
      data.drawable->valide = false;
      data.drawable->compile = false;
    }

    if (data.drawable->valide)
    {
      data.drawable->Draw(frustum, matrix, sun_matrix, shader);
    }

    if (!data.sector.expired())
    {
      auto sector = data.sector.lock();
      if (sector->NeedCompile() /*|| !data.drawable->compile*/)
      {
        data.drawable->compile = true;
        sector->NeedCompile(false);
        mCompilerWorker.Add({ data.sector, data.drawable });
      }

      //sector->Draw(matrix, shader);
    }
  }

  mCompilerWorker.Update();
}

void DrawableArea::UpdateRadius(unsigned int radius)
{
  mData.clear();

  int begin = -static_cast<int>(radius);
  int end = static_cast<int>(radius);
  SPos pos(begin);
  pos.y() = 0;
  for (pos.z() = begin; pos.z() <= end; ++pos.z())
    for (pos.x() = begin; pos.x() <= end; ++pos.x())
    {
      mData.push_back({ pos, SPos{}, std::weak_ptr<Sector>{}, std::make_shared<SectorRenderData>() });
    }
}

void DrawableArea::UpdatePos(const SPos &pos)
{
  for (auto &data : mData)
  {
    data.world_pos = data.local_pos + pos;
    data.drawable->valide = false;
    data.sector.reset();
  }
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

void SectorRenderData::Draw(const Magnum::Frustum &frustum, const Magnum::Matrix4 &matrix, const Magnum::Matrix4 &sun_matrix, Magnum::AbstractShaderProgram& shader)
{
  constexpr const Matrix4 bias
  { { 0.5f, 0.0f, 0.0f, 0.0f },
  { 0.0f, 0.5f, 0.0f, 0.0f },
  { 0.0f, 0.0f, 0.5f, 0.0f },
  { 0.5f, 0.5f, 0.5f, 1.0f } };

  if (Math::Geometry::Intersection::boxFrustum(aabb, frustum))
  {
    auto &std_shader = static_cast<StandartShader &>(shader);
    std_shader.setProjection(matrix * model);
    std_shader.setLightVector(-sun_matrix.backward());
    std_shader.setShadowMatrix(bias * sun_matrix * model);
    mesh.draw(shader);
  }
}


//================== TaskCompile ==================
TaskCompile::TaskCompile(std::weak_ptr<Sector> sector, std::weak_ptr<SectorRenderData> drawable)
{
  mSector = sector;
  mDrawable = drawable;
  if (!mSector.expired())
  {
    mPos = mSector.lock()->GetPos();
  }
}

bool TaskCompile::Begin(SectorCompiler &compiler)
{
  if (!mSector.expired())
  {
    mSector.lock()->SetCompilerData(compiler);
    return true;
  }
  return false;
}

void TaskCompile::End(const SectorCompiler &compiler)
{
  if (!mDrawable.expired())
  {
    auto drawable = mDrawable.lock();
    drawable->SetPos(mPos);
    drawable->vertex_buffer.setData(compiler.GetVertexData(), BufferUsage::StaticDraw);
    drawable->index_buffer.setData(compiler.GetIndexData(), BufferUsage::StaticDraw);
    drawable->mesh.setCount(compiler.GetIndexData().size());
    drawable->valide = true;
  }
}
