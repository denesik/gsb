#include "UpdatableSectors.h"

#include <algorithm>
#include "World.h"

UpdatableSectors::UpdatableSectors(World &world)
  : mWorld(world)
{
}


UpdatableSectors::~UpdatableSectors()
{
}

void UpdatableSectors::Add(const SPos &pos)
{
  auto it = std::find_if(mSectors.begin(), mSectors.end(), [&pos](const decltype(mSectors)::value_type &el) 
  { 
    return std::get<0>(el) == pos; 
  });

  if (it != mSectors.end())
  {
    ++std::get<1>(*it);
  }
  else
  {
    mSectors.emplace_back(pos, 1, std::weak_ptr<Sector>{});
  }
}

void UpdatableSectors::Remove(const SPos &pos)
{
  auto it = std::find_if(mSectors.begin(), mSectors.end(), [&pos](const decltype(mSectors)::value_type &el)
  {
    return std::get<0>(el) == pos;
  });

  if (it != mSectors.end())
  {
    auto &count = std::get<1>(*it);
    --count;
    if (count == 0)
    {
      std::swap(*it, mSectors.back());
      mSectors.resize(mSectors.size() - 1);
      mWorld.UnLoadSector(pos); //TODO: это нужно делать по событию от Unloader
    }
  }
//   else
//   {
//     mWorld.UnLoadSector(pos);
//   }
}

void UpdatableSectors::Update()
{
  const double N = 60.0;
  bool updating = false;
  if (mTimer.Elapsed() > 1.0 / N)
  {
    mTimer.Start();
    updating = true;
  }
  updating = true;

  if (updating)
  for (auto &site : mSectors)
  {
    auto sector = std::get<2>(site).lock();
    if (!sector)
    {
      std::get<2>(site) = mWorld.GetSector(std::get<0>(site));
      sector = std::get<2>(site).lock();
      if (!sector)
      {
        mWorld.LoadSector(std::get<0>(site));
      }
    }

    if (sector)
    {
      sector->Update();
    }
  }
}
