#include "Sector.h"
#include "World.h"
#include "StandartShader.h"

using namespace Magnum;

Sector::Sector(World *world)
  : mWorld(world), mSectorTesselator(world->GetBlocksDataBase())
{
  mStaticBlocks.fill(0);

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

void Sector::Draw(Magnum::AbstractShaderProgram& shader)
{
  mMesh.draw(shader);
}
