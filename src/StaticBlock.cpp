#include "StaticBlock.h"
#include "Model.h"



StaticBlock::StaticBlock()
{
}


StaticBlock::~StaticBlock()
{
}

void StaticBlock::SetTesselator(std::unique_ptr<Tesselator> tesselator)
{
  mTesselator = std::move(tesselator);
}

const std::unique_ptr<Tesselator> & StaticBlock::GetTesselator() const
{
  return mTesselator;
}

void StaticBlock::TestAddModel(const DataBase &db)
{
  mModel = std::make_unique<Model>(db);
}

boost::optional<const Model &> StaticBlock::GetModel() const
{
  if (mModel) return{ *mModel };

  return{};
}
