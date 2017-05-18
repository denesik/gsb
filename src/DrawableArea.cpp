#include "DrawableArea.h"
#include "World.h"
#include "tools\CoordSystem.h"

#include <Magnum/Math/Frustum.h>
#include "StandartShader.h"
#include <Magnum/Math/Geometry/Intersection.h>
#include <Magnum/Math/Frustum.h>

using namespace Magnum;

static int sector_count = 0;

DrawableArea::DrawableArea(World &world)
  : mWorld(world),
  mCompiler(1, 1, world.GetBlocksDataBase()),
  mBufferData(
    [this](const SPos &pos)
    {
      ++sector_count;
      //LOG(trace) << "Draw sector [" << pos.x() << " " << pos.z() << "]. Count: " << sector_count;
      return OnBufferAdd(pos);
    },
    [this](BufferData &data, const SPos &pos)
    {
      --sector_count;
      //LOG(trace) << "Undraw sector [" << pos.x() << " " << pos.z() << "]. Count: " << sector_count;
      OnBufferRemove(data, pos);
    })
{
  mCompiler.SetBeginCallback([this](Worker &worker, SPos &pos)
  {
    return OnCompilerBegin(worker, pos);
  });

  mCompiler.SetEndCallback([this](Worker &worker, SPos &pos)
  {
    return OnCompilerEnd(worker, pos);
  });
}


DrawableArea::~DrawableArea()
{
}

void DrawableArea::SetRadius(unsigned int radius)
{
  mBufferData.SetSize({ static_cast<int>(radius), static_cast<int>(radius) });
}

void DrawableArea::SetPos(const SPos &pos)
{
  mBufferData.UpdatePos(pos);
}

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

  for (auto &data : mBufferData)
  {
    //Каждый кадр бегаем по всем элементам, если сектор нужно скомпилить, отправляем его на компиляцию.
    auto &sector = data.sector_data.get().sector;
    if (sector)
    {
      if (sector->NeedCompile())
      {
        sector->NeedCompile(false);
        mCompiler.Push(sector->GetPos());
        UseSector(sector->GetPos());
      }

      data.drawable.Draw(frustum, matrix, sun_matrix, lightdir, shader);
    }
  }

  mCompiler.Update();
}

bool DrawableArea::OnCompilerBegin(Worker &worker, SPos &pos)
{
  // Если сектор есть, загружаем компилятор данными. Если нету, UnuseSector(pos). return false.
  auto &data = GetSectorData(pos);
  if (data.sector)
  {
    data.sector->SetCompilerData(worker.compiler);
  }
  else
  {
    data.added = false;
    UnuseSector(pos);
    return false;
  }
  return true;
}

bool DrawableArea::OnCompilerEnd(Worker &worker, SPos &pos)
{
  // Если сектор есть, обновляем меш. UnuseSector(pos).
  auto &data = mBufferData.Get(pos);
  if (data && data->sector_data.get().sector)
  {
    auto &drawable = data->drawable;
    drawable.SetPos(pos);
    drawable.vertex_buffer.setData(worker.compiler.GetVertexData(), BufferUsage::StaticDraw);
    drawable.index_buffer.setData(worker.compiler.GetIndexData(), BufferUsage::StaticDraw);
    drawable.mesh.setCount(worker.compiler.GetIndexData().size());
  }
  {
    auto &data = GetSectorData(pos);
    data.added = false;
    UnuseSector(pos);
  }
  return true;
}

DrawableArea::BufferData DrawableArea::OnBufferAdd(const SPos &pos)
{
  // Добавляем элемент в буфер. Если не поставлен на компиляцию, ставим на компиляцию.
  UseSector(pos);

  auto &data = GetSectorData(pos);
  if (data.sector && !data.added)
  {
    data.added = true;
    data.sector->NeedCompile(false);
    mCompiler.Push(pos);
    UseSector(pos);
  }

  return{ data };
}

void DrawableArea::OnBufferRemove(BufferData &data, const SPos &pos)
{
  UnuseSector(pos);
}

static int sector_count1 = 0;

void DrawableArea::Load(Sector &sector)
{
  // Добавляем сектор. Если сектор есть в буфере и не поставлен на компиляцию, ставим на компиляцию.
  const auto &pos = sector.GetPos();
  UseSector(pos);

  auto &data = GetSectorData(pos);
  data.sector = sector;
  if (mBufferData.Get(pos) && !data.added)
  {
    data.added = true;
    sector.NeedCompile(false);
    mCompiler.Push(pos);
    UseSector(pos);
  }
  ++sector_count1;
  //LOG(trace) << "Load sector [" << pos.x() << " " << pos.z() << "]. Count: " << sector_count1;
}

void DrawableArea::UnLoad(Sector &sector)
{
  // Удаляем сектор.
  const auto &pos = sector.GetPos();
  GetSectorData(pos).sector.reset();
  UnuseSector(pos);

  --sector_count1;
  //LOG(trace) << "Unload sector [" << pos.x() << " " << pos.z() << "]. Count: " << sector_count1;
}

static int sector_count2 = 0;
void DrawableArea::UseSector(const SPos &pos)
{
  if (pos == SPos{0, 0, 0})
  {
    ++sector_count2;
    LOG(trace) << "Use sector [" << pos.x() << " " << pos.z() << "]. Count: " << sector_count2;
  }
  auto it = mSectors.find(pos);
  if (it == mSectors.end())
  {
    auto &res = mSectors.emplace(pos, pos);
    if (!res.second)
      return;
    it = res.first;
    // Создали сектор, сообщим ему и всем его соседям об этом.
  }
  ++it->second.count;
}

void DrawableArea::UnuseSector(const SPos &pos)
{
  if (pos == SPos{0, 0, 0})
  {
    --sector_count2;
    LOG(trace) << "Unuse sector [" << pos.x() << " " << pos.z() << "]. Count: " << sector_count2;
  }
  auto it = mSectors.find(pos);
  if (it != mSectors.end())
  {
    --it->second.count;
    if (!it->second.count)
    {
      mSectors.erase(it);
    }
  }
}

DrawableArea::SectorData & DrawableArea::GetSectorData(const SPos &pos)
{
  auto it = mSectors.find(pos);
  return it->second;
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
  m_pos = pos;
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
