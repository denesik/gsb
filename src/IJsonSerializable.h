#pragma once
#include <rapidjson/document.h>
#include <tools/Common.h>

class TextureAtlas;

class GSB_NOVTABLE IJsonSerializable
{
public:
  //virtual ~IJsonSerializable() = default; �� ������������ ��� �������� �����������, ����������� ���������� �� ���������

  virtual void JsonLoad(const rapidjson::Value& val) = 0;
};
