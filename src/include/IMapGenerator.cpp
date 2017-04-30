#include "IMapGenerator.h"

void IMapGenerator::DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx)
{
}

void MapTemplateGenerator::extend_x(size_t layer_offset, size_t count)
{
  auto presize = size;

  size.x() += count;
  data.resize(size.length());
}

void MapTemplateGenerator::extend_y(size_t layer_offset, size_t count)
{
  auto presize = size;

  size.y() += count;
  data.resize(size.length());
}

void MapTemplateGenerator::extend_z(size_t layer_offset, size_t count)
{
  auto presize = size;

  size.z() += count;
  data.resize(size.length());
}

BlockId & MapTemplateGenerator::operator[](SBPos position)
{
  auto norm = position + zero_offset;
  if (norm.x() >= size.x())
    extend_x(size.x() - norm.x() + 1);
  if (norm.y() >= size.y())
    extend_y(size.y() - norm.y() + 1);
  if (norm.z() >= size.z())
    extend_z(size.z() - norm.z() + 1);

  return data[cs::SBtoBIcustom(position, size)];
}

BlockId MapTemplateGenerator::at(SBPos position) const
{
  return BlockId();
}

std::vector<BlockId> MapTemplateGenerator::to_vector()
{
  std::vector<BlockId> v;
  v.reserve(data.size());
  v.insert(v.end(), data.begin(), data.end());
  return v;
}
