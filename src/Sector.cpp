#include "Sector.h"
#include "World.h"
#include "StandartShader.h"

using namespace Magnum;

Sector::Sector(World *world, const SPos &pos)
  : mWorld(world), mSectorTesselator(world->GetBlocksDataBase())
{
  WPos wpos = cs::StoW(pos);
  mModelMatrix = mModelMatrix * Math::Matrix4<Float>::translation(Vector3::xAxis(wpos.x()));
  mModelMatrix = mModelMatrix * Math::Matrix4<Float>::translation(Vector3::yAxis(wpos.y()));
  mModelMatrix = mModelMatrix * Math::Matrix4<Float>::translation(Vector3::zAxis(wpos.z()));
  //mStaticBlocks.fill(0);

  // generate sector
  mStaticBlocks.fill(1);
}


Sector::~Sector()
{
}

bool Sector::NeedCompile() const
{
  return mNeedCompile;
}

void Sector::RunCompiler()
{
  mSectorTesselator.SetMiddle(mStaticBlocks);
  mSectorTesselator.Run();

  mVertexBuffer.setData(mSectorTesselator.vertex_data, BufferUsage::StaticDraw);
  mIndexBuffer.setData(mSectorTesselator.index_data, BufferUsage::StaticDraw);

  mMesh.setPrimitive(MeshPrimitive::Triangles);
  mMesh.addVertexBuffer(mVertexBuffer, 0, StandartShader::Position{}, StandartShader::TextureCoordinates{});
  mMesh.setIndexBuffer(mIndexBuffer, 0, Mesh::IndexType::UnsignedInt);
  mMesh.setCount(mSectorTesselator.index_data.size());

  mNeedCompile = false;
}

void Sector::Draw(const Magnum::Matrix4 &matrix, Magnum::AbstractShaderProgram& shader)
{
  static_cast<StandartShader &>(shader).setProjection(matrix * mModelMatrix);
  mMesh.draw(shader);
}

void Sector::Update()
{

}
