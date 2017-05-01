#include "BlockSpriteAnimated.h"

#include "World.h"
#include "Sector.h"
#include "StaticBlock.h"
#include "Model.h"

BlockSpriteAnimated::BlockSpriteAnimated(const BlockSpriteAnimated &other)
  : BlockSpriteAnimated(other, other.m_sector)
{
}

BlockSpriteAnimated::BlockSpriteAnimated(BlockSpriteAnimated &&other)
  : BlockSpriteAnimated(std::move(other), other.m_sector)
{
}

BlockSpriteAnimated::BlockSpriteAnimated(const BlockSpriteAnimated &other, Sector &parent)
  : Block(other, parent)
{
}

BlockSpriteAnimated::BlockSpriteAnimated(BlockSpriteAnimated &&other, Sector &parent)
  : Block(std::move(other), parent)
{
}

BlockSpriteAnimated::BlockSpriteAnimated(const DataBase &db, const rapidjson::Value &val, Sector &parent, BlockId id)
  : Block(db, val, parent, id)
{
}

std::unique_ptr<Block> BlockSpriteAnimated::Clone(Sector &parent)
{
  return std::make_unique<BlockSpriteAnimated>(*this, parent);
}

void BlockSpriteAnimated::DrawGui(const Magnum::Timeline &dt, GuiCtx & ctx)
{
}

void BlockSpriteAnimated::Update(const Magnum::Timeline &dt)
{
  auto &world = m_sector.GetWorld();
  const auto &player_pos = world.mPlayer.Pos();

  mMovable.SetPos(WPos(cs::SBtoWB(cs::BItoSB(mPos), m_sector.GetPos())) + WPos(0.5f));
  mMovable.LookAt(player_pos);
  mMovable.Update(dt);
}

void BlockSpriteAnimated::Draw(const Magnum::Matrix4 &vp, Magnum::AbstractShaderProgram& shader)
{
  GetStaticPart()->GetModel()->Draw(vp * mMovable.Model(), shader, frame);
}
