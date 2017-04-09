#pragma once
#include <rapidjson/document.h>
#include <tools/Common.h>

class TextureAtlas;
class DataBase;

class GSB_NOVTABLE IJsonSerializable
{
public:
  //virtual ~IJsonSerializable() = default; �� ������������ ��� �������� �����������, ����������� ���������� �� ���������

  virtual void JsonLoad(const DataBase & db, const rapidjson::Value& val) = 0;
};
