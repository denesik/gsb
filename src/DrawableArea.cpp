#include "DrawableArea.h"
#include "World.h"



DrawableArea::DrawableArea(World *world, const SPos &pos, unsigned int radius)
  : mWorld(world), mPos(pos)
{
  UpdateRadius(radius);
  UpdatePos(mPos);
}


DrawableArea::~DrawableArea()
{
}

void DrawableArea::SetPos(const SPos &pos)
{
  if (mPos != pos)
  {
    mPos = pos;
    UpdatePos(mPos);
  }
}

void DrawableArea::Draw(Magnum::AbstractShaderProgram& shader)
{
  for (auto &site : mSectors)
  {
    auto sector = std::get<2>(site).lock();
    if (!sector)
    {
      std::get<2>(site) = mWorld->GetSector(std::get<1>(site));
      sector = std::get<2>(site).lock();
    }

    if (sector)
    {
      // Сектор существует.
      // Перестраиваем геометрию если нужно.
      // Рисуем.
      if (sector->NeedCompile())
      {
        sector->RunCompiler();
      }

      sector->Draw(shader);
    }
  }
}

void DrawableArea::UpdateRadius(unsigned int radius)
{
  mSectors.clear();

  int begin = -static_cast<int>(radius);
  int end = static_cast<int>(radius);
  SPos pos(begin);
  //pos.z = 0;
  for (pos.z() = begin; pos.z() <= end; ++pos.z())
    for (pos.y() = begin; pos.y() <= end; ++pos.y())
      for (pos.x() = begin; pos.x() <= end; ++pos.x())
      {
        mSectors.emplace_back(pos, SPos{}, std::weak_ptr<Sector>{});
      }
}

void DrawableArea::UpdatePos(const SPos &pos)
{
  for (auto &site : mSectors)
  {
    std::get<1>(site) = std::get<0>(site) + pos;
    std::get<2>(site).reset();
  }
}
