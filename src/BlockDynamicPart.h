#pragma once
#ifndef BlockDinamicPart_h__
#define BlockDinamicPart_h__

#include <memory>
#include "Tesselator.h"


class BlockDynamicPart
{
public:
  BlockDynamicPart();
  ~BlockDynamicPart();

  std::unique_ptr<BlockDynamicPart> Clone();

  std::unique_ptr<TesselatorData> mTesselatorData;
};



#endif // BlockDinamicPart_h__
