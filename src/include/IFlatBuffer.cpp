#pragma once
#include <flatbuffers\flatbuffers.h>
#include <tools/Common.h>

class GSB_NOVTABLE IFlatBuffer
{
public:
  //virtual ~IJsonSerializable() = default; �� ������������ ��� �������� �����������, ����������� ���������� �� ���������

  virtual void FlatLoad(void * buff) = 0;
};