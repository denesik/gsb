#pragma once
#include <rapidjson/document.h>
#include <tools/Common.h>

class TextureAtlas;
class BlocksDataBase;

class GSB_NOVTABLE IJsonSerializable
{
public:
  //virtual ~IJsonSerializable() = default; не используетс€ дл€ хранени€ экземпл€ров, виртуальный деструктор не требуетс€

  virtual void JsonLoad(BlocksDataBase & db, const rapidjson::Value& val) = 0;
};
