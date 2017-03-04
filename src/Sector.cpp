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

  mMesh.setPrimitive(MeshPrimitive::Triangles);
  mMesh.addVertexBuffer(mVertexBuffer, 0, StandartShader::Position{}, StandartShader::TextureCoordinates{});
  mMesh.setIndexBuffer(mIndexBuffer, 0, Mesh::IndexType::UnsignedInt);
}


Sector::~Sector()
{
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
  if (auto sector = mWorld.GetSector(cs::Left(mPos)))
    mSectorCompiler->SetSide(sector->mStaticBlocks, SideFlags::LEFT);

  if (auto sector = mWorld.GetSector(cs::Front(mPos)))
    mSectorCompiler->SetSide(sector->mStaticBlocks, SideFlags::FRONT);

  if (auto sector = mWorld.GetSector(cs::Top(mPos)))
    mSectorCompiler->SetSide(sector->mStaticBlocks, SideFlags::TOP);

  if (auto sector = mWorld.GetSector(cs::Right(mPos)))
    mSectorCompiler->SetSide(sector->mStaticBlocks, SideFlags::RIGHT);

  if (auto sector = mWorld.GetSector(cs::Back(mPos)))
    mSectorCompiler->SetSide(sector->mStaticBlocks, SideFlags::BACK);

  if (auto sector = mWorld.GetSector(cs::Bottom(mPos)))
    mSectorCompiler->SetSide(sector->mStaticBlocks, SideFlags::BOTTOM);
  
  mSectorCompiler->Run();
}

void Sector::Draw(const Magnum::Frustum &frustum, const Magnum::Matrix4 &matrix, Magnum::AbstractShaderProgram& shader)
{
  if (mSectorCompiler && mSectorCompiler->IsDone())
  {
    mVertexBuffer.setData(mSectorCompiler->GetVertexData(), BufferUsage::StaticDraw);
    mIndexBuffer.setData(mSectorCompiler->GetIndexData(), BufferUsage::StaticDraw);
    mMesh.setCount(mSectorCompiler->GetIndexData().size());

    mSectorCompiler.reset();
  }

  if (Math::Geometry::Intersection::boxFrustum(mAabb, frustum)) 
  {
    static_cast<StandartShader &>(shader).setProjection(matrix * mModelMatrix);
    mMesh.draw(shader);
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
