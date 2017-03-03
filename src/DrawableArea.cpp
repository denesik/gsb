#include "DrawableArea.h"
#include "World.h"
#include "tools\CoordSystem.h"

#include <Magnum/Math/Frustum.h>

using namespace Magnum;

DrawableArea::DrawableArea(World &world, const SPos &pos, unsigned int radius)
  : mWorld(world), mPos(pos)
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

void DrawableArea::Draw(const Magnum::Matrix4 &matrix, Magnum::AbstractShaderProgram& shader)
{
  // Обновляем список видимых секторов N раз в сек.

  const double N = 10.0;
  bool loading = false;
  if (mTimer.Elapsed() > 1.0 / N)
  {
    mTimer.Start();
    loading = true;
  }

  const Math::Frustum<Float> frustum = Math::Frustum<Float>::fromMatrix(matrix);

  for (auto &site : mSectors)
  {
    auto sector = std::get<2>(site).lock();
    if (loading && !sector)
    {
      std::get<2>(site) = mWorld.GetSector(std::get<1>(site));
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

      sector->Draw(frustum, matrix, shader);
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
