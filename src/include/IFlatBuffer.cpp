#pragma once
#include <flatbuffers\flatbuffers.h>
#include <tools/Common.h>

class GSB_NOVTABLE IFlatBuffer
{
public:
  //virtual ~IJsonSerializable() = default; не используетс€ дл€ хранени€ экземпл€ров, виртуальный деструктор не требуетс€

  virtual void FlatLoad(void * buff) = 0;
};