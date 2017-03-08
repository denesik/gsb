#pragma once
#ifndef BlockDinamicPart_h__
#define BlockDinamicPart_h__

#include <memory>
#include "Tesselator.h"
#include "agent/Agent.h"


class BlockDynamicPart
{
public:
  BlockDynamicPart();
  ~BlockDynamicPart();

  std::unique_ptr<BlockDynamicPart> Clone();

  std::unique_ptr<TesselatorData> mTesselatorData;

  std::vector<std::unique_ptr<Agent>> mAgents;
};



#endif // BlockDinamicPart_h__
