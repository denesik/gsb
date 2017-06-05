#include "AccessorHeat.h"
#include "../Serialize.h"

AccessorHeat::AccessorHeat(const AccessorHeat &other)
  : ParentType(other)
  , mMax(other.mMax)
  , mCollected(other.mCollected)
{
}

AccessorHeat::AccessorHeat(AccessorHeat &&other)
  : ParentType(std::move(other))
  , mMax(other.mMax)
  , mCollected(other.mCollected)
{
}

AccessorHeat::AccessorHeat(const AccessorHeat &other, Block &parent)
  : ParentType(other, parent)
  , mMax(other.mMax)
  , mCollected(other.mCollected)
{
}

AccessorHeat::AccessorHeat(AccessorHeat &&other, Block &parent)
  : ParentType(std::move(other), parent)
  , mMax(other.mMax)
  , mCollected(other.mCollected)
{
}

AccessorHeat::AccessorHeat(const DataBase &db, const rapidjson::Value &val, Block &parent)
  : ParentType(db, val, parent)
{
  JSONLOAD(sge::make_nvp("max", mMax));
  JSONLOAD(sge::make_nvp("collected", mCollected));
}

void AccessorHeat::DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx, IContext & context)
{
}
