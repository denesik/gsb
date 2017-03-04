#include "Sector.h"
#include "World.h"
#include "StandartShader.h"
#include <Magnum/Math/Geometry/Intersection.h>
#include "MapGenerator.h"

using namespace Magnum;

Sector::Sector(World &world, const SPos &pos)
  : mWorld(world), mPos(pos)
{
  WPos wpos = cs::StoW(pos);
  mModelMatrix = mModelMatrix * Math::Matrix4<Float>::translation(Vector3::xAxis(wpos.x()));
  mModelMatrix = mModelMatrix * Math::Matrix4<Float>::translation(Vector3::yAxis(wpos.y()));
  mModelMatrix = mModelMatrix * Math::Matrix4<Float>::translation(Vector3::zAxis(wpos.z()));
  //mStaticBlocks.fill(0);

  mAabb.min() = wpos;
  mAabb.max() = wpos + WPos(static_cast<WPosType>(SECTOR_SIZE));

  // generate sector
  mStaticBlocks.fill(2);
}


Sector::~Sector()
{
}

void Sector::CreateRenderData()
{
  mRenderData = std::make_unique<RenderData>();
  mRenderData->mesh.setPrimitive(MeshPrimitive::Triangles);
  mRenderData->mesh.addVertexBuffer(mRenderData->vertex_buffer, 0, StandartShader::Position{}, StandartShader::TextureCoordinates{});
  mRenderData->mesh.setIndexBuffer(mRenderData->index_buffer, 0, Mesh::IndexType::UnsignedInt);
}

bool Sector::NeedCompile() const
{
  return mNeedCompile;
}

bool Sector::Compiling() const
{
  return mSectorCompiler != nullptr;
}

void Sector::RunCompiler(std::shared_ptr<SectorCompiler> sectorCompiler)
{
  if (mSectorCompiler)
  {
    return;
  }

  mSectorCompiler = sectorCompiler;
  mNeedCompile = false;

  mSectorCompiler->SetMiddle(mStaticBlocks);
  if (auto sector = mWorld.GetSector(cs::Left(mPos)).lock())
    mSectorCompiler->SetSide(sector->mStaticBlocks, SideFlags::LEFT);

  if (auto sector = mWorld.GetSector(cs::Front(mPos)).lock())
    mSectorCompiler->SetSide(sector->mStaticBlocks, SideFlags::FRONT);

  if (auto sector = mWorld.GetSector(cs::Top(mPos)).lock())
    mSectorCompiler->SetSide(sector->mStaticBlocks, SideFlags::TOP);

  if (auto sector = mWorld.GetSector(cs::Right(mPos)).lock())
    mSectorCompiler->SetSide(sector->mStaticBlocks, SideFlags::RIGHT);

  if (auto sector = mWorld.GetSector(cs::Back(mPos)).lock())
    mSectorCompiler->SetSide(sector->mStaticBlocks, SideFlags::BACK);

  if (auto sector = mWorld.GetSector(cs::Bottom(mPos)).lock())
    mSectorCompiler->SetSide(sector->mStaticBlocks, SideFlags::BOTTOM);
  
  mSectorCompiler->Run();
}

void Sector::Draw(const Magnum::Frustum &frustum, const Magnum::Matrix4 &matrix, Magnum::AbstractShaderProgram& shader)
{
  if (!mRenderData)
    return;

  if (mSectorCompiler && mSectorCompiler->IsDone())
  {
    mRenderData->vertex_buffer.setData(mSectorCompiler->GetVertexData(), BufferUsage::StaticDraw);
    mRenderData->index_buffer.setData(mSectorCompiler->GetIndexData(), BufferUsage::StaticDraw);
    mRenderData->mesh.setCount(mSectorCompiler->GetIndexData().size());

    mSectorCompiler.reset();
  }

  if (Math::Geometry::Intersection::boxFrustum(mAabb, frustum)) 
  {
    static_cast<StandartShader &>(shader).setProjection(matrix * mModelMatrix);
    mRenderData->mesh.draw(shader);
  }
}

void Sector::Update()
{

}

SPos Sector::GetPos() const
{
  return mPos;
}

void Sector::ApplyGenerator(IMapGenerator &generator)
{

  generator.Generate(*this, mStaticBlocks);
}

BlockId Sector::GetBlockId(SBPos pos) const
{
  return mStaticBlocks[cs::SBtoBI(pos)];
}

void Sector::SetBlockId(SBPos pos, BlockId id)
{
  mStaticBlocks[cs::SBtoBI(pos)] = id;
  mNeedCompile = true;
}
