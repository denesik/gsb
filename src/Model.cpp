#include "Model.h"
#include "StandartShader.h"
#include "DataBase.h"
#include <vector>
#include "Tesselator.h"
#include <string>

static const Magnum::Vector3 gVertexData[] =
{
  {  0.3f, -0.3f, 0.0f }, //FLB
  { -0.3f, -0.3f, 0.0f }, //FRB
  { -0.3f,  0.3f, 0.0f }, //FRT
  {  0.3f,  0.3f, 0.0f }, //FLT
};

static const Magnum::Vector2 gTextureSide[] =
{
  { 0.0f + 0.1f, 0.0f + 0.1f },
  { 1.0f - 0.1f, 0.0f + 0.1f },
  { 1.0f - 0.1f, 1.0f - 0.1f },
  { 0.0f + 0.1f, 1.0f - 0.1f },
};

static const Magnum::UnsignedInt gIndexSide[] =
{
  0, 1, 2, 2, 3, 0
};

Model::Model(const DataBase &db)
{
  std::vector<TesselatorVertex> vertex;
  std::vector<Magnum::UnsignedInt> index;

  const auto &atlas = db.GetAtlas();
  for (size_t i = 0; i < 16; ++i)
  {
    const auto &coord = *atlas.GetTextureCoord(std::string("data/number_") + std::to_string(i) + ".tga");
    for (size_t j = 0; j < 4; ++j)
    {
      vertex.emplace_back(gVertexData[j], Magnum::Vector2{
        gTextureSide[j].x() * (coord.right() - coord.left()) + coord.left(),
        gTextureSide[j].y() * (coord.top() - coord.bottom()) + coord.bottom()
      });
    }

    data[i].mesh.setPrimitive(MeshPrimitive::Triangles);
    data[i].mesh.addVertexBuffer(data[i].vertex_buffer, 0, StandartShader::Position{}, StandartShader::TextureCoordinates{});
    data[i].mesh.setIndexBuffer(data[i].index_buffer, 0, Mesh::IndexType::UnsignedInt);

    data[i].vertex_buffer.setData(vertex, BufferUsage::StaticDraw);
    data[i].index_buffer.setData(gIndexSide, BufferUsage::StaticDraw);
    data[i].mesh.setCount(6);
  }
}

void Model::Draw(const Magnum::Matrix4 &mvp, Magnum::AbstractShaderProgram& shader, size_t frame) const
{
  static_cast<StandartShader &>(shader).setProjection(mvp);
  data[frame].mesh.draw(shader);
}
