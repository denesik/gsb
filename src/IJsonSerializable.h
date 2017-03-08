#pragma once
#include <rapidjson/document.h>
#include <tools/Common.h>

class TextureAtlas;
class BlocksDataBase;

class GSB_NOVTABLE IJsonSerializable
{
public:
  //virtual ~IJsonSerializable() = default; �� ������������ ��� �������� �����������, ����������� ���������� �� ���������

  virtual void JsonLoad(BlocksDataBase & db, const rapidjson::Value& val) = 0;
};
