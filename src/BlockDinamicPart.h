#pragma once
#ifndef BlockDinamicPart_h__
#define BlockDinamicPart_h__

#include <memory>
#include "Tesselator.h"


class BlockDinamicPart
{
public:
  BlockDinamicPart();
  ~BlockDinamicPart();

  std::unique_ptr<TesselatorData> mTesselatorData;
};



#endif // BlockDinamicPart_h__
